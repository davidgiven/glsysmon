// Off-screen UI renderer shared by the visual-snapshot test harnesses: draws
// one frame of an injected Ui into an SDL_GPU texture and writes it as a PNG.
//
// The caller supplies a Fruit component that provides Ui and the shared
// ImGuiFrameRenderer; tests can bind fake sensors/views in that component.

#pragma once

#include <fruit/fruit.h>

#include <string>

#include "imgui_frame_renderer.h"
#include "preferences.h"
#include "ui.h"

namespace render_frame
{

    // A Fruit component providing the Ui and the shared frame renderer.
    using Components = fruit::Component<Ui, ImGuiFrameRenderer> (*)(CliArgs*);

    // Renders one frame at the given pixel size into `output` as a PNG. If
    // `height` is <= 0 the primary display's usable height is used. Returns 0
    // on success, non-zero on failure.
    int RenderFrame(int width,
        int height,
        const std::string& output,
        Components components,
        CliArgs* args);

} // namespace render_frame
