#include "dock.h"

#include <SDL3/SDL.h>
#include <wayland-client.h>

// The generated client header uses `namespace` as a parameter name, which is a
// C++ keyword; rename it for the duration of the include.
#define namespace namespace_
#include "wayland/wlr-layer-shell-client-protocol.h"
#undef namespace

#include <cstring>

namespace {

uint32_t AnchorForSide(const std::string &side)
{
    const uint32_t vertical =
        ZWLR_LAYER_SURFACE_V1_ANCHOR_TOP | ZWLR_LAYER_SURFACE_V1_ANCHOR_BOTTOM;
    const uint32_t horizontal =
        ZWLR_LAYER_SURFACE_V1_ANCHOR_LEFT | ZWLR_LAYER_SURFACE_V1_ANCHOR_RIGHT;
    if (side == "left")
        return ZWLR_LAYER_SURFACE_V1_ANCHOR_LEFT | vertical;
    if (side == "right")
        return ZWLR_LAYER_SURFACE_V1_ANCHOR_RIGHT | vertical;
    if (side == "top")
        return ZWLR_LAYER_SURFACE_V1_ANCHOR_TOP | horizontal;
    return ZWLR_LAYER_SURFACE_V1_ANCHOR_BOTTOM | horizontal;
}

class WaylandDockImpl : public Dock {
public:
    explicit WaylandDockImpl(const Preferences &prefs) : _prefs(&prefs) {}

    SDL_Window *CreateWindow() override
    {
        // Create the window as a roleless wl_surface; SDL will not assign a
        // shell role, so we can attach the layer-shell role before any buffer
        // is attached.
        const std::string side = GlobalPreferencesFetcher::GetSide(*_prefs);
        const int size = GlobalPreferencesFetcher::GetSize(*_prefs);
        const int monitor = GlobalPreferencesFetcher::GetMonitor(*_prefs);

        SDL_PropertiesID props = SDL_CreateProperties();
        SDL_SetBooleanProperty(
            props, SDL_PROP_WINDOW_CREATE_WAYLAND_SURFACE_ROLE_CUSTOM_BOOLEAN, true);
        SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER, size);
        SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER, size);
        SDL_Window *window = SDL_CreateWindowWithProperties(props);
        SDL_DestroyProperties(props);
        if (window == nullptr)
            return nullptr;

        _display = static_cast<wl_display *>(SDL_GetPointerProperty(
            SDL_GetWindowProperties(window), SDL_PROP_WINDOW_WAYLAND_DISPLAY_POINTER, nullptr));
        _surface = static_cast<wl_surface *>(SDL_GetPointerProperty(
            SDL_GetWindowProperties(window), SDL_PROP_WINDOW_WAYLAND_SURFACE_POINTER, nullptr));
        if (_display == nullptr || _surface == nullptr) {
            SDL_SetError("Wayland window surface properties unavailable");
            SDL_DestroyWindow(window);
            return nullptr;
        }

        // SDL owns the display; a second registry listener is allowed.
        wl_registry *registry = wl_display_get_registry(_display);
        wl_registry_add_listener(registry, &_registryListener, this);
        wl_display_roundtrip(_display);
        wl_registry_destroy(registry);
        if (_shell == nullptr) {
            SDL_SetError("compositor does not advertise zwlr_layer_shell_v1");
            SDL_DestroyWindow(window);
            return nullptr;
        }

        SDL_DisplayID display_id = DockGetDisplay(monitor);
        wl_output *output = static_cast<wl_output *>(SDL_GetPointerProperty(
            SDL_GetDisplayProperties(display_id), SDL_PROP_DISPLAY_WAYLAND_WL_OUTPUT_POINTER,
            nullptr));

        _layerSurface = zwlr_layer_shell_v1_get_layer_surface(
            _shell, _surface, output, ZWLR_LAYER_SHELL_V1_LAYER_TOP, "glrellm");
        if (_layerSurface == nullptr) {
            SDL_SetError("zwlr_layer_shell_v1_get_layer_surface failed");
            SDL_DestroyWindow(window);
            return nullptr;
        }
        zwlr_layer_surface_v1_add_listener(_layerSurface, &_layerSurfaceListener, this);

        const bool vertical = (side == "left" || side == "right");
        zwlr_layer_surface_v1_set_anchor(_layerSurface, AnchorForSide(side));
        // size 0 on the unconstrained axis lets the compositor stretch the dock.
        zwlr_layer_surface_v1_set_size(_layerSurface, vertical ? static_cast<uint32_t>(size) : 0,
                                       vertical ? 0 : static_cast<uint32_t>(size));
        zwlr_layer_surface_v1_set_exclusive_zone(_layerSurface, size);
        // EXCLUSIVE so ImGui receives keyboard input on the layer surface.
        zwlr_layer_surface_v1_set_keyboard_interactivity(
            _layerSurface, ZWLR_LAYER_SURFACE_V1_KEYBOARD_INTERACTIVITY_EXCLUSIVE);
        wl_surface_commit(_surface);
        wl_display_flush(_display);

        return window;
    }

    void PollWindow(SDL_Window *window) override
    {
        if (_layerSurface == nullptr)
            return;
        if (_pendingWidth == 0 || _pendingHeight == 0)
            return;
        // Keep the swapchain in sync with the compositor-assigned surface size.
        SDL_SetWindowSize(window, static_cast<int>(_pendingWidth),
                          static_cast<int>(_pendingHeight));
        _pendingWidth = 0;
        _pendingHeight = 0;
    }

private:
    static void HandleConfigure(void *data, zwlr_layer_surface_v1 *surface,
                                uint32_t serial, uint32_t width, uint32_t height)
    {
        WaylandDockImpl *dock = static_cast<WaylandDockImpl *>(data);
        zwlr_layer_surface_v1_ack_configure(surface, serial);
        if (width > 0 && height > 0) {
            dock->_pendingWidth = width;
            dock->_pendingHeight = height;
        }
    }

    static void HandleClosed(void *, zwlr_layer_surface_v1 *) {}

    static void HandleGlobal(void *data, wl_registry *registry, uint32_t name,
                             const char *interface, uint32_t version)
    {
        WaylandDockImpl *dock = static_cast<WaylandDockImpl *>(data);
        if (std::strcmp(interface, zwlr_layer_shell_v1_interface.name) == 0) {
            dock->_shell = static_cast<zwlr_layer_shell_v1 *>(wl_registry_bind(
                registry, name, &zwlr_layer_shell_v1_interface, version));
        }
    }

    static void HandleGlobalRemove(void *, wl_registry *, uint32_t) {}

    const Preferences *_prefs;
    wl_display *_display = nullptr;
    wl_surface *_surface = nullptr;
    zwlr_layer_shell_v1 *_shell = nullptr;
    zwlr_layer_surface_v1 *_layerSurface = nullptr;
    uint32_t _pendingWidth = 0;
    uint32_t _pendingHeight = 0;

    static const wl_registry_listener _registryListener;
    static const zwlr_layer_surface_v1_listener _layerSurfaceListener;
};

const wl_registry_listener WaylandDockImpl::_registryListener = {
    WaylandDockImpl::HandleGlobal,
    WaylandDockImpl::HandleGlobalRemove,
};

const zwlr_layer_surface_v1_listener WaylandDockImpl::_layerSurfaceListener = {
    WaylandDockImpl::HandleConfigure,
    WaylandDockImpl::HandleClosed,
};

}  // namespace

std::unique_ptr<Dock> DockCreateWayland(const Preferences &prefs)
{
    return std::make_unique<WaylandDockImpl>(prefs);
}
