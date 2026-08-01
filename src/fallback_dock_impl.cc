#include "dock.h"

namespace {

class FallbackDockImpl : public Dock {
public:
    explicit FallbackDockImpl(DockConfig cfg) : _cfg(std::move(cfg)) {}

    SDL_Window *CreateWindow() override
    {
        SDL_DisplayID display = DockGetDisplay(_cfg.monitor);
        SDL_Rect bounds;
        if (!SDL_GetDisplayUsableBounds(display, &bounds))
            return nullptr;

        int width = _cfg.size;
        int height = _cfg.size;
        int x = bounds.x;
        int y = bounds.y;
        if (_cfg.side == "left" || _cfg.side == "right") {
            height = bounds.h;
            if (_cfg.side == "right")
                x = bounds.x + bounds.w - width;
        } else {
            width = bounds.w;
            if (_cfg.side == "bottom")
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
    DockConfig _cfg;
};

}  // namespace

std::unique_ptr<Dock> DockCreateFallback(const DockConfig &cfg)
{
    return std::make_unique<FallbackDockImpl>(cfg);
}
