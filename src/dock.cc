#include "dock.h"

#include <fruit/fruit.h>

#include <cstring>

#include "components.h"

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

std::unique_ptr<Dock> DockCreate(const Preferences &prefs)
{
    const char *driver = SDL_GetCurrentVideoDriver();
    if (driver == nullptr)
        return DockCreateFallback(prefs);
    if (std::strcmp(driver, "wayland") == 0)
        return DockCreateWayland(prefs);
    if (std::strcmp(driver, "x11") == 0)
        return DockCreateX11(prefs);
    return DockCreateFallback(prefs);
}

fruit::Component<fruit::Required<CliArgs>, DockFactory> GetDockComponent()
{
    return fruit::createComponent()
        .install(GetPreferencesComponent)
        .registerFactory<std::unique_ptr<Dock>(Preferences *)>(
            [](Preferences *prefs) { return DockCreate(*prefs); });
}
