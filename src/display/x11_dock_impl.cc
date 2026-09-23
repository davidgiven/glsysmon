#include "dock.h"

#include <X11/Xatom.h>
#include <X11/Xlib.h>

#include <cstring>
#include <unistd.h>

namespace
{

    class X11DockImpl : public Dock
    {
    public:
        explicit X11DockImpl(const Preferences& prefs): _prefs(&prefs) {}

        SDL_Window* CreateWindow() override
        {
            const std::string side = GlobalPreferencesFetcher::GetSide(*_prefs);
            const int size = GlobalPreferencesFetcher::GetSize(*_prefs);
            const int monitor = GlobalPreferencesFetcher::GetMonitor(*_prefs);

            SDL_DisplayID display = DockGetDisplay(monitor);
            SDL_Rect bounds;
            if (!SDL_GetDisplayBounds(display, &bounds))
                return nullptr;

            int width = size;
            int height = bounds.h;
            int x = bounds.x;
            int y = bounds.y;
            if (side == "right")
                x = bounds.x + bounds.w - width;

            SDL_SetHint(SDL_HINT_X11_WINDOW_TYPE, "_NET_WM_WINDOW_TYPE_DOCK");
            SDL_Window* window = SDL_CreateWindow("glsysmon",
                width,
                height,
                SDL_WINDOW_BORDERLESS | SDL_WINDOW_ALWAYS_ON_TOP |
                    SDL_WINDOW_UTILITY);
            SDL_SetHint(SDL_HINT_X11_WINDOW_TYPE, nullptr);
            if (window == nullptr)
                return nullptr;
            SDL_SetWindowPosition(window, x, y);
            SDL_SetWindowSize(window, width, height);

            SDL_PropertiesID props = SDL_GetWindowProperties(window);
            Display* display_x11 = static_cast<Display*>(SDL_GetPointerProperty(
                props, SDL_PROP_WINDOW_X11_DISPLAY_POINTER, nullptr));
            Window win = static_cast<Window>(SDL_GetNumberProperty(
                props, SDL_PROP_WINDOW_X11_WINDOW_NUMBER, 0));
            if (display_x11 == nullptr || win == 0)
            {
                SDL_SetError("X11 window properties unavailable");
                SDL_DestroyWindow(window);
                return nullptr;
            }

            // Make window sticky / visible on all virtual desktops.
            // SDL's UTILITY + DOCK hint covers SKIP_TASKBAR/SKIP_PAGER
            // and window type, but STICKY/DESKTOP must be set via EWMH.
            // Use ClientMessage so the window manager applies it even
            // after the window is mapped; also set the property directly
            // for WMs that read it.
            {
                Atom net_wm_state =
                    XInternAtom(display_x11, "_NET_WM_STATE", False);
                Atom sticky =
                    XInternAtom(display_x11, "_NET_WM_STATE_STICKY", False);
                XEvent xev;
                std::memset(&xev, 0, sizeof(xev));
                xev.xclient.type = ClientMessage;
                xev.xclient.display = display_x11;
                xev.xclient.window = win;
                xev.xclient.message_type = net_wm_state;
                xev.xclient.format = 32;
                xev.xclient.data.l[0] = 1; // _NET_WM_STATE_ADD
                xev.xclient.data.l[1] = static_cast<long>(sticky);
                xev.xclient.data.l[2] = 0;
                xev.xclient.data.l[3] = 1; // source indication: normal app
                XSendEvent(display_x11,
                    DefaultRootWindow(display_x11),
                    False,
                    SubstructureRedirectMask | SubstructureNotifyMask,
                    &xev);

                Atom net_wm_desktop =
                    XInternAtom(display_x11, "_NET_WM_DESKTOP", False);
                long desktop = 0xFFFFFFFF;
                XChangeProperty(display_x11,
                    win,
                    net_wm_desktop,
                    XA_CARDINAL,
                    32,
                    PropModeReplace,
                    reinterpret_cast<const unsigned char*>(&desktop),
                    1);
                XEvent xev2;
                std::memset(&xev2, 0, sizeof(xev2));
                xev2.xclient.type = ClientMessage;
                xev2.xclient.display = display_x11;
                xev2.xclient.window = win;
                xev2.xclient.message_type = net_wm_desktop;
                xev2.xclient.format = 32;
                xev2.xclient.data.l[0] = 0xFFFFFFFF;
                xev2.xclient.data.l[1] = 1;
                XSendEvent(display_x11,
                    DefaultRootWindow(display_x11),
                    False,
                    SubstructureRedirectMask | SubstructureNotifyMask,
                    &xev2);
            }

            Atom wm_pid = XInternAtom(display_x11, "_NET_WM_PID", False);
            unsigned long pid = static_cast<unsigned long>(getpid());
            XChangeProperty(display_x11,
                win,
                wm_pid,
                XA_CARDINAL,
                32,
                PropModeReplace,
                reinterpret_cast<const unsigned char*>(&pid),
                1);

            long strut[12] = {};
            long strut_simple[4] = {};
            const long start_y = bounds.y;
            const long end_y = bounds.y + bounds.h - 1;
            if (side == "right")
            {
                strut[1] = size;
                strut_simple[1] = size;
                strut[6] = start_y;
                strut[7] = end_y;
            }
            else
            {
                strut[0] = x + width;
                strut_simple[0] = x + width;
                strut[4] = start_y;
                strut[5] = end_y;
            }
            Atom wm_strut = XInternAtom(display_x11, "_NET_WM_STRUT", False);
            XChangeProperty(display_x11,
                win,
                wm_strut,
                XA_CARDINAL,
                32,
                PropModeReplace,
                reinterpret_cast<const unsigned char*>(strut_simple),
                4);
            Atom wm_strut_partial =
                XInternAtom(display_x11, "_NET_WM_STRUT_PARTIAL", False);
            XChangeProperty(display_x11,
                win,
                wm_strut_partial,
                XA_CARDINAL,
                32,
                PropModeReplace,
                reinterpret_cast<const unsigned char*>(strut),
                12);

            XFlush(display_x11);
            return window;
        }

        void PollWindow(SDL_Window*) override {}

    private:
        const Preferences* _prefs;
    };

} // namespace

std::unique_ptr<Dock> DockCreateX11(const Preferences& prefs)
{
    return std::make_unique<X11DockImpl>(prefs);
}
