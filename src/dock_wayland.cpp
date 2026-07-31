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

struct WaylandDock {
    wl_display *display = nullptr;
    wl_surface *surface = nullptr;
    zwlr_layer_shell_v1 *shell = nullptr;
    zwlr_layer_surface_v1 *layer_surface = nullptr;
    uint32_t pending_width = 0;
    uint32_t pending_height = 0;
};

WaylandDock g_dock;

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

const zwlr_layer_surface_v1_listener kLayerSurfaceListener = {
    // configure
    [](void *data, zwlr_layer_surface_v1 *surface, uint32_t serial,
       uint32_t width, uint32_t height) {
        WaylandDock *dock = static_cast<WaylandDock *>(data);
        zwlr_layer_surface_v1_ack_configure(surface, serial);
        if (width > 0 && height > 0) {
            dock->pending_width = width;
            dock->pending_height = height;
        }
    },
    // closed
    [](void *, zwlr_layer_surface_v1 *) {},
};

const wl_registry_listener kRegistryListener = {
    // global
    [](void *data, wl_registry *registry, uint32_t name,
       const char *interface, uint32_t version) {
        WaylandDock *dock = static_cast<WaylandDock *>(data);
        if (std::strcmp(interface, zwlr_layer_shell_v1_interface.name) == 0) {
            dock->shell = static_cast<zwlr_layer_shell_v1 *>(wl_registry_bind(
                registry, name, &zwlr_layer_shell_v1_interface, version));
        }
    },
    // global_remove
    [](void *, wl_registry *, uint32_t) {},
};

}  // namespace

SDL_Window *DockCreateWindowWayland(const DockConfig &cfg)
{
    // Create the window as a roleless wl_surface; SDL will not assign a shell
    // role, so we can attach the layer-shell role before any buffer is attached.
    SDL_PropertiesID props = SDL_CreateProperties();
    SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_WAYLAND_SURFACE_ROLE_CUSTOM_BOOLEAN, true);
    SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER, cfg.size);
    SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER, cfg.size);
    SDL_Window *window = SDL_CreateWindowWithProperties(props);
    SDL_DestroyProperties(props);
    if (window == nullptr)
        return nullptr;

    g_dock.display = static_cast<wl_display *>(SDL_GetPointerProperty(
        SDL_GetWindowProperties(window), SDL_PROP_WINDOW_WAYLAND_DISPLAY_POINTER, nullptr));
    g_dock.surface = static_cast<wl_surface *>(SDL_GetPointerProperty(
        SDL_GetWindowProperties(window), SDL_PROP_WINDOW_WAYLAND_SURFACE_POINTER, nullptr));
    if (g_dock.display == nullptr || g_dock.surface == nullptr) {
        SDL_SetError("Wayland window surface properties unavailable");
        SDL_DestroyWindow(window);
        return nullptr;
    }

    // SDL owns the display; a second registry listener is allowed.
    wl_registry *registry = wl_display_get_registry(g_dock.display);
    wl_registry_add_listener(registry, &kRegistryListener, &g_dock);
    wl_display_roundtrip(g_dock.display);
    wl_registry_destroy(registry);
    if (g_dock.shell == nullptr) {
        SDL_SetError("compositor does not advertise zwlr_layer_shell_v1");
        SDL_DestroyWindow(window);
        return nullptr;
    }

    SDL_DisplayID display_id = DockGetDisplay(cfg.monitor);
    wl_output *output = static_cast<wl_output *>(SDL_GetPointerProperty(
        SDL_GetDisplayProperties(display_id), SDL_PROP_DISPLAY_WAYLAND_WL_OUTPUT_POINTER, nullptr));

    g_dock.layer_surface = zwlr_layer_shell_v1_get_layer_surface(
        g_dock.shell, g_dock.surface, output, ZWLR_LAYER_SHELL_V1_LAYER_TOP, "glrellm");
    if (g_dock.layer_surface == nullptr) {
        SDL_SetError("zwlr_layer_shell_v1_get_layer_surface failed");
        SDL_DestroyWindow(window);
        return nullptr;
    }
    zwlr_layer_surface_v1_add_listener(g_dock.layer_surface, &kLayerSurfaceListener, &g_dock);

    const bool vertical = (cfg.side == "left" || cfg.side == "right");
    zwlr_layer_surface_v1_set_anchor(g_dock.layer_surface, AnchorForSide(cfg.side));
    // size 0 on the unconstrained axis lets the compositor stretch the dock.
    zwlr_layer_surface_v1_set_size(g_dock.layer_surface,
                                   vertical ? static_cast<uint32_t>(cfg.size) : 0,
                                   vertical ? 0 : static_cast<uint32_t>(cfg.size));
    zwlr_layer_surface_v1_set_exclusive_zone(g_dock.layer_surface, cfg.size);
    // EXCLUSIVE so ImGui receives keyboard input on the layer surface.
    zwlr_layer_surface_v1_set_keyboard_interactivity(
        g_dock.layer_surface, ZWLR_LAYER_SURFACE_V1_KEYBOARD_INTERACTIVITY_EXCLUSIVE);
    wl_surface_commit(g_dock.surface);
    wl_display_flush(g_dock.display);

    return window;
}

void DockWaylandApplyPendingSize(SDL_Window *window)
{
    if (g_dock.layer_surface == nullptr)
        return;
    if (g_dock.pending_width == 0 || g_dock.pending_height == 0)
        return;
    // Keep the swapchain in sync with the compositor-assigned surface size.
    SDL_SetWindowSize(window, static_cast<int>(g_dock.pending_width),
                      static_cast<int>(g_dock.pending_height));
    g_dock.pending_width = 0;
    g_dock.pending_height = 0;
}
