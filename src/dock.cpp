#include "dock.h"

#include <cstring>

SDL_DisplayID DockGetDisplay(int index)
{
    int count = 0;
    SDL_DisplayID *displays = SDL_GetDisplays(&count);
    if (displays == nullptr)
        return SDL_GetPrimaryDisplay();
    const SDL_DisplayID result =
        (index < count) ? displays[index] : SDL_GetPrimaryDisplay();
    SDL_free(displays);
    return result;
}

SDL_Window *DockCreateWindow(const DockConfig &cfg)
{
    const char *driver = SDL_GetCurrentVideoDriver();
    if (driver == nullptr)
        return DockCreateWindowFallback(cfg);
    if (std::strcmp(driver, "wayland") == 0)
        return DockCreateWindowWayland(cfg);
    if (std::strcmp(driver, "x11") == 0)
        return DockCreateWindowX11(cfg);
    return DockCreateWindowFallback(cfg);
}

void DockPollWindow(SDL_Window *window)
{
    const char *driver = SDL_GetCurrentVideoDriver();
    if (driver != nullptr && std::strcmp(driver, "wayland") == 0)
        DockWaylandApplyPendingSize(window);
}

SDL_Window *DockCreateWindowFallback(const DockConfig &cfg)
{
    SDL_DisplayID display = DockGetDisplay(cfg.monitor);
    SDL_Rect bounds;
    if (!SDL_GetDisplayUsableBounds(display, &bounds))
        return nullptr;

    int width = cfg.size;
    int height = cfg.size;
    int x = bounds.x;
    int y = bounds.y;
    if (cfg.side == "left" || cfg.side == "right") {
        height = bounds.h;
        if (cfg.side == "right")
            x = bounds.x + bounds.w - width;
    } else {
        width = bounds.w;
        if (cfg.side == "bottom")
            y = bounds.y + bounds.h - height;
    }

    SDL_Window *window = SDL_CreateWindow(
        "glrellm", width, height,
        SDL_WINDOW_BORDERLESS | SDL_WINDOW_ALWAYS_ON_TOP);
    if (window == nullptr)
        return nullptr;
    SDL_SetWindowPosition(window, x, y);
    SDL_SetWindowSize(window, width, height);
    return window;
}
