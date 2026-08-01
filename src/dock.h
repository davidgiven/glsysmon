#pragma once

#include <SDL3/SDL.h>

#include <functional>
#include <memory>
#include <string>

struct DockConfig {
    std::string side = "left";  // "left" | "right" | "top" | "bottom"
    int size = 240;             // dock width (left/right) or height (top/bottom)
    int monitor = 0;            // SDL display index
};

// Dock backend interface. Implementations live in dock_x11.cpp /
// dock_wayland.cpp / dock.cpp and are selected via DockCreate().
class Dock {
public:
    virtual ~Dock() = default;

    // Creates the application window and docks it to a screen edge, using the
    // config the backend was constructed with. Must be called before the first
    // swapchain present. Returns nullptr on failure (SDL_GetError() set).
    virtual SDL_Window *CreateWindow() = 0;

    // Applies compositor-driven size changes (Wayland layer-surface configure).
    // Call once per frame after event pumping.
    virtual void PollWindow(SDL_Window *window) = 0;
};

// Injected into components that need a Dock for a runtime-specific config.
using DockFactory = std::function<std::unique_ptr<Dock>(DockConfig)>;

// Resolves a display index to an SDL_DisplayID, falling back to primary.
SDL_DisplayID DockGetDisplay(int index);

// Backend entry points; used by GetDockComponent() so app.cpp stays
// platform-agnostic.
std::unique_ptr<Dock> DockCreate(const DockConfig &cfg);
std::unique_ptr<Dock> DockCreateX11(const DockConfig &cfg);
std::unique_ptr<Dock> DockCreateWayland(const DockConfig &cfg);
std::unique_ptr<Dock> DockCreateFallback(const DockConfig &cfg);
