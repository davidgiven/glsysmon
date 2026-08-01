#include "dock.h"

#include <SDL3/SDL.h>
#include <wayland-client.h>

// The generated client header uses `namespace` as a parameter name, which is a
// C++ keyword; rename it for the duration of the include.
#define namespace namespace_
#include "wayland/wlr-layer-shell-client-protocol.h"
#undef namespace

#include <cstring>
#include <utility>

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
    explicit WaylandDockImpl(DockConfig cfg) : cfg_(std::move(cfg)) {}

    SDL_Window *CreateWindow() override
    {
        // Create the window as a roleless wl_surface; SDL will not assign a
        // shell role, so we can attach the layer-shell role before any buffer
        // is attached.
        SDL_PropertiesID props = SDL_CreateProperties();
        SDL_SetBooleanProperty(
            props, SDL_PROP_WINDOW_CREATE_WAYLAND_SURFACE_ROLE_CUSTOM_BOOLEAN, true);
        SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER, cfg_.size);
        SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER, cfg_.size);
        SDL_Window *window = SDL_CreateWindowWithProperties(props);
        SDL_DestroyProperties(props);
        if (window == nullptr)
            return nullptr;

        display_ = static_cast<wl_display *>(SDL_GetPointerProperty(
            SDL_GetWindowProperties(window), SDL_PROP_WINDOW_WAYLAND_DISPLAY_POINTER, nullptr));
        surface_ = static_cast<wl_surface *>(SDL_GetPointerProperty(
            SDL_GetWindowProperties(window), SDL_PROP_WINDOW_WAYLAND_SURFACE_POINTER, nullptr));
        if (display_ == nullptr || surface_ == nullptr) {
            SDL_SetError("Wayland window surface properties unavailable");
            SDL_DestroyWindow(window);
            return nullptr;
        }

        // SDL owns the display; a second registry listener is allowed.
        wl_registry *registry = wl_display_get_registry(display_);
        wl_registry_add_listener(registry, &kRegistryListener, this);
        wl_display_roundtrip(display_);
        wl_registry_destroy(registry);
        if (shell_ == nullptr) {
            SDL_SetError("compositor does not advertise zwlr_layer_shell_v1");
            SDL_DestroyWindow(window);
            return nullptr;
        }

        SDL_DisplayID display_id = DockGetDisplay(cfg_.monitor);
        wl_output *output = static_cast<wl_output *>(SDL_GetPointerProperty(
            SDL_GetDisplayProperties(display_id), SDL_PROP_DISPLAY_WAYLAND_WL_OUTPUT_POINTER,
            nullptr));

        layer_surface_ = zwlr_layer_shell_v1_get_layer_surface(
            shell_, surface_, output, ZWLR_LAYER_SHELL_V1_LAYER_TOP, "glrellm");
        if (layer_surface_ == nullptr) {
            SDL_SetError("zwlr_layer_shell_v1_get_layer_surface failed");
            SDL_DestroyWindow(window);
            return nullptr;
        }
        zwlr_layer_surface_v1_add_listener(layer_surface_, &kLayerSurfaceListener, this);

        const bool vertical = (cfg_.side == "left" || cfg_.side == "right");
        zwlr_layer_surface_v1_set_anchor(layer_surface_, AnchorForSide(cfg_.side));
        // size 0 on the unconstrained axis lets the compositor stretch the dock.
        zwlr_layer_surface_v1_set_size(layer_surface_,
                                       vertical ? static_cast<uint32_t>(cfg_.size) : 0,
                                       vertical ? 0 : static_cast<uint32_t>(cfg_.size));
        zwlr_layer_surface_v1_set_exclusive_zone(layer_surface_, cfg_.size);
        // EXCLUSIVE so ImGui receives keyboard input on the layer surface.
        zwlr_layer_surface_v1_set_keyboard_interactivity(
            layer_surface_, ZWLR_LAYER_SURFACE_V1_KEYBOARD_INTERACTIVITY_EXCLUSIVE);
        wl_surface_commit(surface_);
        wl_display_flush(display_);

        return window;
    }

    void PollWindow(SDL_Window *window) override
    {
        if (layer_surface_ == nullptr)
            return;
        if (pending_width_ == 0 || pending_height_ == 0)
            return;
        // Keep the swapchain in sync with the compositor-assigned surface size.
        SDL_SetWindowSize(window, static_cast<int>(pending_width_),
                          static_cast<int>(pending_height_));
        pending_width_ = 0;
        pending_height_ = 0;
    }

private:
    static void HandleConfigure(void *data, zwlr_layer_surface_v1 *surface,
                                uint32_t serial, uint32_t width, uint32_t height)
    {
        WaylandDockImpl *dock = static_cast<WaylandDockImpl *>(data);
        zwlr_layer_surface_v1_ack_configure(surface, serial);
        if (width > 0 && height > 0) {
            dock->pending_width_ = width;
            dock->pending_height_ = height;
        }
    }

    static void HandleClosed(void *, zwlr_layer_surface_v1 *) {}

    static void HandleGlobal(void *data, wl_registry *registry, uint32_t name,
                             const char *interface, uint32_t version)
    {
        WaylandDockImpl *dock = static_cast<WaylandDockImpl *>(data);
        if (std::strcmp(interface, zwlr_layer_shell_v1_interface.name) == 0) {
            dock->shell_ = static_cast<zwlr_layer_shell_v1 *>(wl_registry_bind(
                registry, name, &zwlr_layer_shell_v1_interface, version));
        }
    }

    static void HandleGlobalRemove(void *, wl_registry *, uint32_t) {}

    DockConfig cfg_;
    wl_display *display_ = nullptr;
    wl_surface *surface_ = nullptr;
    zwlr_layer_shell_v1 *shell_ = nullptr;
    zwlr_layer_surface_v1 *layer_surface_ = nullptr;
    uint32_t pending_width_ = 0;
    uint32_t pending_height_ = 0;

    static const wl_registry_listener kRegistryListener;
    static const zwlr_layer_surface_v1_listener kLayerSurfaceListener;
};

const wl_registry_listener WaylandDockImpl::kRegistryListener = {
    WaylandDockImpl::HandleGlobal,
    WaylandDockImpl::HandleGlobalRemove,
};

const zwlr_layer_surface_v1_listener WaylandDockImpl::kLayerSurfaceListener = {
    WaylandDockImpl::HandleConfigure,
    WaylandDockImpl::HandleClosed,
};

}  // namespace

std::unique_ptr<Dock> DockCreateWayland(const DockConfig &cfg)
{
    return std::make_unique<WaylandDockImpl>(cfg);
}
