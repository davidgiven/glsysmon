#pragma once

#include <SDL3/SDL.h>

#include <string>

struct DockConfig {
    std::string side = "left";  // "left" | "right" | "top" | "bottom"
    int size = 240;             // dock width (left/right) or height (top/bottom)
    int monitor = 0;            // SDL display index
};

// Creates the application window and docks it to a screen edge using the
// backend for SDL_GetCurrentVideoDriver(). Must be called before the first
// swapchain present. Returns nullptr on failure (SDL_GetError() set).
SDL_Window *DockCreateWindow(const DockConfig &cfg);

// Resolves a display index to an SDL_DisplayID, falling back to primary.
SDL_DisplayID DockGetDisplay(int index);

// Applies compositor-driven size changes (Wayland layer-surface configure).
// Call once per frame after event pumping.
void DockPollWindow(SDL_Window *window);

// Backend entry points; exposed here so app.cpp stays platform-agnostic.
SDL_Window *DockCreateWindowX11(const DockConfig &cfg);
SDL_Window *DockCreateWindowWayland(const DockConfig &cfg);
SDL_Window *DockCreateWindowFallback(const DockConfig &cfg);
void DockWaylandApplyPendingSize(SDL_Window *window);
