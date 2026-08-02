#include "dock.h"

namespace {

class FallbackDockImpl : public Dock {
public:
    explicit FallbackDockImpl(const Preferences &prefs) : _prefs(&prefs) {}

    SDL_Window *CreateWindow() override
    {
        const std::string side = GlobalPreferencesFetcher::GetSide(*_prefs);
        const int size = GlobalPreferencesFetcher::GetSize(*_prefs);
        const int monitor = GlobalPreferencesFetcher::GetMonitor(*_prefs);

        SDL_DisplayID display = DockGetDisplay(monitor);
        SDL_Rect bounds;
        if (!SDL_GetDisplayUsableBounds(display, &bounds))
            return nullptr;

        int width = size;
        int height = bounds.h;
        int x = bounds.x;
        int y = bounds.y;
        if (side == "right")
            x = bounds.x + bounds.w - width;

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
    const Preferences *_prefs;
};

}  // namespace

std::unique_ptr<Dock> DockCreateFallback(const Preferences &prefs)
{
    return std::make_unique<FallbackDockImpl>(prefs);
}
