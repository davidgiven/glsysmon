// Helper for the visual-snapshot tests: renders one frame of an injected Ui
// into an off-screen texture and compares the resulting PNG against the
// golden reference. Consolidates the RenderFrame + ImagesMatch + logging
// boilerplate duplicated across the render_fake_*.cc tests.

#pragma once

#include <string>

#include "display/imgui_frame_renderer.h"
#include "ui.h"

namespace render_lib
{

    int Run(const std::string& name,
        Ui& ui,
        ImGuiFrameRenderer& renderer,
        int width = 240,
        int height = 0);

} // namespace render_lib
