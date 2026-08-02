#include "dock.h"

#include <X11/Xatom.h>
#include <X11/Xlib.h>

#include <unistd.h>

namespace {

class X11DockImpl : public Dock {
public:
    explicit X11DockImpl(const Preferences &prefs) : _prefs(&prefs) {}

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
        int height = size;
        int x = bounds.x;
        int y = bounds.y;
        if (side == "left" || side == "right") {
            height = bounds.h;
            if (side == "right")
                x = bounds.x + bounds.w - width;
        } else {
            width = bounds.w;
            if (side == "bottom")
                y = bounds.y + bounds.h - height;
        }

        SDL_Window *window = SDL_CreateWindow(
            "glrellm", width, height,
            SDL_WINDOW_BORDERLESS | SDL_WINDOW_ALWAYS_ON_TOP);
        if (window == nullptr)
            return nullptr;
        SDL_SetWindowPosition(window, x, y);
        SDL_SetWindowSize(window, width, height);

        SDL_PropertiesID props = SDL_GetWindowProperties(window);
        Display *display_x11 = static_cast<Display *>(
            SDL_GetPointerProperty(props, SDL_PROP_WINDOW_X11_DISPLAY_POINTER, nullptr));
        Window win = static_cast<Window>(
            SDL_GetNumberProperty(props, SDL_PROP_WINDOW_X11_WINDOW_NUMBER, 0));
        if (display_x11 == nullptr || win == 0) {
            SDL_SetError("X11 window properties unavailable");
            SDL_DestroyWindow(window);
            return nullptr;
        }

        Atom wm_window_type = XInternAtom(display_x11, "_NET_WM_WINDOW_TYPE", False);
        Atom dock_type = XInternAtom(display_x11, "_NET_WM_WINDOW_TYPE_DOCK", False);
        XChangeProperty(display_x11, win, wm_window_type, XA_ATOM, 32, PropModeReplace,
                        reinterpret_cast<const unsigned char *>(&dock_type), 1);

        Atom wm_state = XInternAtom(display_x11, "_NET_WM_STATE", False);
        Atom sticky = XInternAtom(display_x11, "_NET_WM_STATE_STICKY", False);
        Atom above = XInternAtom(display_x11, "_NET_WM_STATE_ABOVE", False);
        Atom states[2] = {sticky, above};
        XChangeProperty(display_x11, win, wm_state, XA_ATOM, 32, PropModeReplace,
                        reinterpret_cast<const unsigned char *>(states), 2);

        Atom wm_pid = XInternAtom(display_x11, "_NET_WM_PID", False);
        unsigned long pid = static_cast<unsigned long>(getpid());
        XChangeProperty(display_x11, win, wm_pid, XA_CARDINAL, 32, PropModeReplace,
                        reinterpret_cast<const unsigned char *>(&pid), 1);

        // _NET_WM_STRUT_PARTIAL: [left, right, top, bottom, ...edge extents...]
        long strut[12] = {};
        const long edge_end =
            (side == "left" || side == "right") ? bounds.h - 1 : bounds.w - 1;
        if (side == "left") {
            strut[0] = size;
            strut[4] = 0;
            strut[5] = edge_end;
        } else if (side == "right") {
            strut[1] = size;
            strut[6] = 0;
            strut[7] = edge_end;
        } else if (side == "top") {
            strut[2] = size;
            strut[8] = 0;
            strut[9] = edge_end;
        } else {
            strut[3] = size;
            strut[10] = 0;
            strut[11] = edge_end;
        }
        Atom wm_strut_partial = XInternAtom(display_x11, "_NET_WM_STRUT_PARTIAL", False);
        XChangeProperty(display_x11, win, wm_strut_partial, XA_CARDINAL, 32, PropModeReplace,
                        reinterpret_cast<const unsigned char *>(strut), 12);

        XFlush(display_x11);
        return window;
    }

    void PollWindow(SDL_Window *) override {}

private:
    const Preferences *_prefs;
};

}  // namespace

std::unique_ptr<Dock> DockCreateX11(const Preferences &prefs)
{
    return std::make_unique<X11DockImpl>(prefs);
}
