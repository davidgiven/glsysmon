#pragma once

#include <SDL3/SDL.h>

#include <functional>
#include <memory>

#include "preferences.h"

// Dock backend interface. Implementations live in x11_dock_impl.cc /
// wayland_dock_impl.cc / fallback_dock_impl.cc and are selected via
// DockCreate().
class Dock
{
public:
    virtual ~Dock() = default;

    // Creates the application window and docks it to a screen edge, using the
    // preferences the backend was constructed with. Must be called before the
    // first swapchain present. Returns nullptr on failure (SDL_GetError() set).
    virtual SDL_Window* CreateWindow() = 0;

    // Applies compositor-driven size changes (Wayland layer-surface configure).
    // Call once per frame after event pumping.
    virtual void PollWindow(SDL_Window* window) = 0;
};

// Factory for creating a Dock; the backend is chosen from the active video
// driver and its preferences are taken from the Preferences passed to
// CreateDockFactory.
using DockFactory = std::function<std::unique_ptr<Dock>()>;

// Resolves a display index to an SDL_DisplayID, falling back to primary.
extern SDL_DisplayID DockGetDisplay(int index);

// Backend entry points; used by CreateDockFactory() so app.cc stays
// platform-agnostic.
extern std::unique_ptr<Dock> DockCreate(const Preferences& prefs);
extern std::unique_ptr<Dock> DockCreateX11(const Preferences& prefs);
extern std::unique_ptr<Dock> DockCreateWayland(const Preferences& prefs);
extern std::unique_ptr<Dock> DockCreateFallback(const Preferences& prefs);
