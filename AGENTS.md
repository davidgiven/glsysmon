# AGENTS.md

Project-specific instructions for AI coding agents.

## Overview

glrellm is a C++ desktop widget that docks to a screen edge. It uses SDL3
(window, input, and rendering via SDL_GPU) and Dear ImGui for the UI. The window
is docked to the screen using X11 struts (`_NET_WM_STRUT_PARTIAL`) on X11 and the
Wayland `wlr-layer-shell` protocol on Wayland. Current scope is an application
skeleton; system monitoring comes later.

## Build

Requires (Debian packages): `libsdl3-dev`, `libimgui-dev`, `libstb-dev`,
`libwayland-dev`, `libx11-dev`, `libvulkan-dev`, `libgoogle-fruit-dev`,
`pkg-config`.

```sh
make          # build the glrellm binary
make run      # build and run (defaults: left edge, 240px, primary monitor)
make clean
```

The build uses pkg-config for `sdl3`, `imgui`, `x11`, and `wayland-client`.
Fruit ships no `.pc` file, so it is linked as `-lfruit`. ImGui core is linked
from the Debian package (`-limgui`); the platform/render backends are compiled
from the package's source tree at
`/usr/share/doc/libimgui-dev/examples/backends/` (`imgui_impl_sdl3.cpp`,
`imgui_impl_sdlgpu3.cpp`). Wayland client stubs for the layer-shell protocol are
generated at build time by `wayland-scanner` from
`third_party/wayland/wlr-layer-shell-unstable-v1.xml`.

Run/lint: C++20, g++, `-Wall -Wextra`. No test framework or formatter is set up.

## Architecture

- `src/main.cpp` — CLI args (`--side`, `--size`, `--monitor`); the composition
  root builds a Fruit `Injector<App>` and runs it.
- `src/components.h` — declares the module `Get*Component()` functions; the
  only header that includes `<fruit/fruit.h>`.
- `src/app.h` / `src/app.cpp` — `App` interface; `ImGuiAppImpl` (SDL init,
  SDL_GPU device, ImGui setup, main render loop) receives `DockFactory` and
  `Ui` via Fruit constructor injection; `GetAppComponent()` installs the Dock
  and Ui components.
- `src/dock.h` — `Dock` interface (`CreateWindow()`, `PollWindow()`), the
  `DockFactory` alias, and the backend factory entry points
  `DockCreate{X11,Wayland,Fallback}`.
- `src/dock.cpp` — `FallbackDockImpl`, `DockCreate()` dispatch on
  `SDL_GetCurrentVideoDriver()`, and `GetDockComponent()`, which binds
  `DockFactory` through assisted injection of `DockConfig`.
- `src/dock_x11.cpp` — `X11DockImpl`; sets `_NET_WM_WINDOW_TYPE`=DOCK,
  `_NET_WM_STRUT_PARTIAL`, `_NET_WM_STATE`=ABOVE|STICKY via Xlib using
  `SDL_PROP_WINDOW_X11_*`.
- `src/dock_wayland.cpp` — `WaylandDockImpl`; creates the SDL window as a
  roleless Wayland surface
  (`SDL_PROP_WINDOW_CREATE_WAYLAND_SURFACE_ROLE_CUSTOM_BOOLEAN`), extracts the
  `wl_surface`/`wl_display` from window properties, and attaches a
  `zwlr_layer_surface_v1` role (anchor, exclusive zone, keyboard interactivity).
- `src/ui.h` / `src/ui.cpp` — `Ui` interface; `ImGuiUiImpl` widgets;
  `GetUiComponent()`.

## Conventions

- Keep platform-specific code in `dock_*.cpp` behind the `dock.h` interface;
  no X11/Wayland types may leak into `app.cpp` or `ui.cpp`.
- Headers declare pure-virtual interfaces only; implementations are
  `*Impl` classes defined in `.cpp` files, one module component function
  (`Get*Component()`) per module, declared in `components.h`. Fruit types must
  not appear in interface headers.
- Wire components with Fruit dependency injection: interfaces via
  `.bind<I, Impl>()` (impls expose a `using Inject = Impl(...)` typedef),
  runtime-config dependencies via assisted injection through a
  `std::function` factory.
- Name class data members with a leading underscore (`_name`); local
  variables and function parameters stay bare.
- The dock backend must be configured before the first swapchain present.
- On Wayland, the layer-surface `configure` event must call `ack_configure`
  and then `SDL_SetWindowSize()` so the SDL swapchain matches.
- No comments unless they explain a non-obvious protocol/API requirement.
- Prefer Debian system libraries over vendored copies. The only vendored file
  is the layer-shell protocol XML (not packaged by Debian).

## Gotchas

- With `imgui_impl_sdlgpu3`, `ImGui_ImplSDLGPU3_PrepareDrawData()` must be
  called BEFORE beginning the render pass that draws ImGui.
- SDL3's Wayland backend owns the `wl_display`; do not create a second
  connection, and add registry listeners (e.g. for `zwlr_layer_shell_v1`) on the
  display returned by `SDL_PROP_WINDOW_WAYLAND_DISPLAY_POINTER`.
- `SDL_HINT_VIDEO_WAYLAND_SHELL_LAYER` and
  `SDL_PROP_WINDOW_CREATE_WAYLAND_LAYER_SHELL_*` do NOT exist in SDL3 — the
  roleless-surface + own layer-shell client approach is the supported one.
