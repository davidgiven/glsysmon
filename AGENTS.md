# AGENTS.md

Project-specific instructions for AI coding agents.

## Overview

glsysmon is a C++ desktop widget that docks to a screen edge. It uses SDL3
(window, input, and rendering via SDL_GPU) and Dear ImGui for the UI. The window
is docked to the screen using X11 struts (`_NET_WM_STRUT_PARTIAL`) on X11 and the
Wayland `wlr-layer-shell` protocol on Wayland. Current scope is an application
skeleton; system monitoring comes later.

## Build

Requires (Debian packages): `libsdl3-dev`, `libstb-dev`,
`libwayland-dev`, `libx11-dev`, `libvulkan-dev`, `doctest-dev`, `pkg-config`.

```sh
make          # build the glsysmon binary
make run      # build and run (defaults: left edge, 240px, primary monitor)
make test     # build and run unit tests, then write the off-screen render snapshot
make compile_commands.json   # clangd compilation database (regenerates on source change)
make clean
```

Tests live in `tests/`. `tests/unit_tests.cc` is a doctest runner
(`DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN`); the header-only framework needs no link
flags. `tests/render_frame.h`/`tests/render_frame.cc` are a library
(`render_frame::RenderFrame`) that draws one frame of an injected `Ui` into an
SDL_GPU off-screen texture and saves it as a PNG; it does not exercise the dock
backends. The caller supplies the `Ui` and `ImGuiFrameRenderer` directly
(e.g. `tests/render_fake_hostname.cc` creates a `Ui` with only `HostnameView`
enabled using a fake `HostnameSensor` via `CreateUiWithFakeHostname`), writing
`tests/render_fake_hostname.bad.png` (deliberately outside `.obj` for easy
inspection; gitignored) and comparing it pixel by pixel against the tracked
golden reference `tests/render_fake_hostname.good.png` (passes when identical;
regenerate the golden by copying the `.bad.png` over it). All test binaries
link against `.obj/imgui_ui_impl.o` and the renderer impl.

The build uses pkg-config for `sdl3`, `x11`, `wayland-client`, and
`tomlplusplus`. ImGui sources are vendored in `dep/imgui` (core:
`imgui.cpp`, `imgui_draw.cpp`, `imgui_tables.cpp`, `imgui_widgets.cpp`;
backends: `dep/imgui/backends/imgui_impl_sdl3.cpp`,
`dep/imgui/backends/imgui_impl_sdlgpu3.cpp`). Wayland client stubs for the
layer-shell protocol are generated at build time by `wayland-scanner` from
`third_party/wayland/wlr-layer-shell-unstable-v1.xml`.

Run/lint: C++20, g++, `-Wall -Wextra`. No test framework or formatter is set up.

## Architecture

- `src/main.cc` — validates CLI args (`--side`, `--size`, `--monitor`,
  `--views`, `--help`); heap-allocates a `CliArgs` and passes it to
  `CreateApp()`, then runs the main loop over `App::Setup()`/`Tick()`/`Shutdown()`;
  catches `std::exception` from `Setup()` and exits non-zero.
- `src/components.h` — declares the manual factory functions `Create*()` for
  each module (e.g. `CreatePreferences()`, `CreateUi()`, `CreateApp()`).
- `src/app.h` — `App` interface (`Setup()`, `Tick()`, `Shutdown()`).
- `src/imgui_app_impl.cc` — `ImGuiAppImpl` (SDL init, SDL_GPU device, one frame
  per `Tick()`, teardown) receives `DockFactory`, `Ui`, and `ImGuiFrameRenderer`
  via manual constructor injection; `Setup()` throws `std::runtime_error` on
  failure and SDL resources are owned by local RAII guards (an SDL session, the
  dock window, and the GPU device), so a partial setup is unwound safely;
  `Shutdown()` releases the guards and is re-entered by the destructor;
  `CreateApp()` builds the preferences, dock factory, Ui and frame renderer
  with the `CliArgs`.
- `src/imgui_frame_renderer.h` — `ImGuiFrameRenderer` interface owning the ImGui
  context and backends (`Init()`, `ProcessEvent()`, `BeginFrame()`, `Render()`
  into any target texture, `Shutdown()`); shared by the app (swapchain target)
  and the off-screen render harness (texture target).
- `src/imgui_frame_renderer_impl.cc` — `ImGuiFrameRendererImpl` (ImGui context
  + IO/style/DPI setup, SDL3/SDLGPU3 backend init, `ImGui::Render` and the
  clear-to-render pass); `CreateImGuiFrameRenderer()`.
- `src/dock.h` — `Dock` interface (`CreateWindow()`, `PollWindow()`), the
  `DockFactory` alias, and the backend factory entry points
  `DockCreate{X11,Wayland,Fallback}`, which take the injected `Preferences`.
- `src/dock.cc` — `DockGetDisplay()`, `DockCreate()` dispatch on
  `SDL_GetCurrentVideoDriver()`, and `CreateDock()`/`CreateDockFactory()`,
  which create the dock using the injected `Preferences`.
- `src/x11_dock_impl.cc` — `X11DockImpl`; sets `_NET_WM_WINDOW_TYPE`=DOCK,
  `_NET_WM_STRUT_PARTIAL`, `_NET_WM_STATE`=ABOVE|STICKY via Xlib using
  `SDL_PROP_WINDOW_X11_*`.
- `src/wayland_dock_impl.cc` — `WaylandDockImpl`; creates the SDL window as a
  roleless Wayland surface
  (`SDL_PROP_WINDOW_CREATE_WAYLAND_SURFACE_ROLE_CUSTOM_BOOLEAN`), extracts the
  `wl_surface`/`wl_display` from window properties, and attaches a
  `zwlr_layer_surface_v1` role (anchor, exclusive zone, keyboard interactivity).
- `src/fallback_dock_impl.cc` — `FallbackDockImpl`, used when no X11/Wayland
  backend is active.
- `src/preferences.h` — `Preferences` interface (a generic key/value store:
  `GetString()`, `GetInteger()`, `GetStringList()`, returning
  `std::optional`), `GlobalPreferencesFetcher`, which provides typed inline
  static accessors (`GetSide()`, `GetSize()`, `GetMonitor()`, `GetViews()`,
  defaulting to `{"HostnameView"}`) over a `Preferences`, and `CliArgs`, a
  wrapper around the argv vector.
- `src/cli_preferences_impl.cc` — `CliPreferencesImpl`, a `Preferences` whose
  values come from `--side=`/`--size=`/`--monitor=`/`--views=` arguments
  (`--views=` is a comma-separated list); `CreateCliPreferences()`.
- `src/toml_preferences_impl.cc` — `TomlPreferencesImpl`, a key/value
  `Preferences` backed by a TOML file at `$XDG_CONFIG_HOME/glsysmon/config.toml`;
  `CreateTomlPreferences()`.
- `src/combined_preferences_impl.cc` — `CombinedPreferencesImpl`, which merges
  the CLI and TOML sources (CLI wins); `CreatePreferences()`.
- `src/ui.h` — `Ui` interface.
- `src/imgui_ui_impl.cc` — `ImGuiUiImpl` takes the injected `Preferences`, looks
  up each configured view name in the view catalogue, builds a view via the
  `ViewFactory` and calls `Tick()` on each during `Draw`; `CreateUi()`.
- `src/view.h` — `View` interface for a system-monitor widget (`Tick()` redraws
  it into the active ImGui window).
- `src/sensor.h` — `Sensor` interface for something that fetches system data
  (no methods yet).
- `src/sensors/hostname_sensor.h` — `HostnameSensor` interface,
  `GetHostname()` returns the current hostname as a `std::string`.
- `src/sensors/hostname_sensor_impl.cc` — `HostnameSensorImpl` reads it via
  `gethostname()`; `CreateHostnameSensor()`.
- `src/views/hostname_view_impl.cc` — `HostnameViewImpl` fetches the hostname
  via an injected `HostnameSensor` and prints it; `CreateHostnameView()`,
  used by `CreateUi()`. Includes from `src/views/`/`src/sensors/`
  use the `src/`-relative path (e.g. `sensors/hostname_sensor.h`), not a bare
  filename.
- `src/views/catalogue.h`/`src/views/catalogue.cc` — `GetViewCatalogue()`, a
  static map of view names to their `ViewFactory` functions, used to load the
  views to display at run time (currently just `HostnameView`).

## Conventions

- Keep platform-specific code in `dock_*.cc` behind the `dock.h` interface;
  no X11/Wayland types may leak into `app.cc` or `ui.cc`.
- Headers declare pure-virtual interfaces only; implementations are
  `*Impl` classes defined in `.cc` files, one manual factory function
  (`Create*()`) per module, declared in `components.h`.
- Name class data members with a leading underscore (`_name`); local
  variables and function parameters stay bare.
- Declare non-static free-function prototypes in headers with `extern`.
- Reformat changed sources with `clang-format` (per the repo `.clang-format`)
  before committing.
- For very large numbers (e.g. nanosecond values) always use `'` digit separators grouped as `000'000` (e.g. `1'000'000'000`, `100'000'000`).
- The dock backend must be configured before the first swapchain present.
- On Wayland, the layer-surface `configure` event must call `ack_configure`
  and then `SDL_SetWindowSize()` so the SDL swapchain matches.
- No comments unless they explain a non-obvious protocol/API requirement.
- Prefer Debian system libraries over vendored copies. The only vendored files
  are the layer-shell protocol XML (not packaged by Debian) and the ImGui
  sources in `dep/imgui` (trimmed to the required backends).

## Gotchas
- With `imgui_impl_sdlgpu3`, `ImGui_ImplSDLGPU3_PrepareDrawData()` must be
  called BEFORE beginning the render pass that draws ImGui.
- SDL3's Wayland backend owns the `wl_display`; do not create a second
  connection, and add registry listeners (e.g. for `zwlr_layer_shell_v1`) on the
  display returned by `SDL_PROP_WINDOW_WAYLAND_DISPLAY_POINTER`.
- `SDL_HINT_VIDEO_WAYLAND_SHELL_LAYER` and
  `SDL_PROP_WINDOW_CREATE_WAYLAND_LAYER_SHELL_*` do NOT exist in SDL3 — the
  roleless-surface + own layer-shell client approach is the supported one.
