#include "dock.h"

#include <fruit/fruit.h>

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

std::unique_ptr<Dock> DockCreate(const DockConfig &cfg)
{
    const char *driver = SDL_GetCurrentVideoDriver();
    if (driver == nullptr)
        return DockCreateFallback(cfg);
    if (std::strcmp(driver, "wayland") == 0)
        return DockCreateWayland(cfg);
    if (std::strcmp(driver, "x11") == 0)
        return DockCreateX11(cfg);
    return DockCreateFallback(cfg);
}

fruit::Component<DockFactory> GetDockComponent()
{
    return fruit::createComponent()
        .registerFactory<std::unique_ptr<Dock>(fruit::Assisted<DockConfig>)>(
            [](DockConfig cfg) { return DockCreate(cfg); });
}
