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

            // Ask SDL to tag the window as a dock. This ONLY takes effect if no
            // other flag (TOOLTIP/UTILITY/POPUP_MENU) claims a specific type --
            // so those flags must NOT be passed to SDL_CreateWindow below.
            SDL_SetHint(SDL_HINT_X11_WINDOW_TYPE, "_NET_WM_WINDOW_TYPE_DOCK");

            // Create hidden so we can finish setting properties before the WM
            // ever sees the window mapped.
            SDL_Window* window = SDL_CreateWindow("glsysmon",
                width,
                height,
                SDL_WINDOW_BORDERLESS | SDL_WINDOW_ALWAYS_ON_TOP |
                    SDL_WINDOW_HIDDEN);
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

            // Initial _NET_WM_STATE: skip taskbar/pager (previously implied by
            // SDL_WINDOW_UTILITY, now set explicitly) + sticky (visible on all
            // virtual desktops). Set as a property (not a ClientMessage) since
            // the window isn't mapped yet -- the WM will pick this up when it
            // maps the window.
            {
                Atom net_wm_state =
                    XInternAtom(display_x11, "_NET_WM_STATE", False);
                Atom skip_taskbar = XInternAtom(
                    display_x11, "_NET_WM_STATE_SKIP_TASKBAR", False);
                Atom skip_pager =
                    XInternAtom(display_x11, "_NET_WM_STATE_SKIP_PAGER", False);
                Atom sticky =
                    XInternAtom(display_x11, "_NET_WM_STATE_STICKY", False);

                Atom states[3] = {skip_taskbar, skip_pager, sticky};
                XChangeProperty(display_x11,
                    win,
                    net_wm_state,
                    XA_ATOM,
                    32,
                    PropModeReplace,
                    reinterpret_cast<const unsigned char*>(states),
                    3);
            }

            // Initial desktop: all desktops (0xFFFFFFFF), set directly as a
            // property rather than a ClientMessage, for the same reason as
            // above.
            {
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

            // Now that window type, state, desktop, PID and struts are all set,
            // it's safe to map the window.
            SDL_ShowWindow(window);
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
