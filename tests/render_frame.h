// Off-screen UI renderer shared by the visual-snapshot test harnesses: draws
// one frame of an injected Ui into an SDL_GPU texture and writes it as a PNG.

#pragma once

#include <string>

#include "imgui_frame_renderer.h"
#include "ui.h"

namespace render_frame
{

    // Renders one frame at the given pixel size into `output` as a PNG. If
    // `height` is <= 0 the primary display's usable height is used. Returns 0
    // on success, non-zero on failure. The caller supplies the Ui and frame
    // renderer (e.g. with a fake sensor for the test).
    int RenderFrame(int width,
        int height,
        const std::string& output,
        Ui& ui,
        ImGuiFrameRenderer& frameRenderer);

    // Returns true if the two PNG files are pixel-identical (same dimensions
    // and bytes). Used to compare a fresh capture against a golden reference.
    bool ImagesMatch(const char* actual, const char* expected);

} // namespace render_frame
