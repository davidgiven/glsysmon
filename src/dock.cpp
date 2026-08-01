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

namespace {

class FallbackDockImpl : public Dock {
public:
    explicit FallbackDockImpl(DockConfig cfg) : cfg_(std::move(cfg)) {}

    SDL_Window *CreateWindow() override
    {
        SDL_DisplayID display = DockGetDisplay(cfg_.monitor);
        SDL_Rect bounds;
        if (!SDL_GetDisplayUsableBounds(display, &bounds))
            return nullptr;

        int width = cfg_.size;
        int height = cfg_.size;
        int x = bounds.x;
        int y = bounds.y;
        if (cfg_.side == "left" || cfg_.side == "right") {
            height = bounds.h;
            if (cfg_.side == "right")
                x = bounds.x + bounds.w - width;
        } else {
            width = bounds.w;
            if (cfg_.side == "bottom")
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

    void PollWindow(SDL_Window *) override {}

private:
    DockConfig cfg_;
};

}  // namespace

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

std::unique_ptr<Dock> DockCreateFallback(const DockConfig &cfg)
{
    return std::make_unique<FallbackDockImpl>(cfg);
}

fruit::Component<DockFactory> GetDockComponent()
{
    return fruit::createComponent()
        .registerFactory<std::unique_ptr<Dock>(fruit::Assisted<DockConfig>)>(
            [](DockConfig cfg) { return DockCreate(cfg); });
}
