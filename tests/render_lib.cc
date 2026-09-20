#include "render_lib.h"

#include <string>

#include <SDL3/SDL.h>

#include "render_frame.h"

namespace render_lib
{

    int Run(const std::string& name,
        Ui& ui,
        ImGuiFrameRenderer& renderer,
        int width,
        int height)
    {
        const std::string bad = "tests/" + name + ".bad.png";
        const std::string good = "tests/" + name + ".good.png";
        const int render_result =
            render_frame::RenderFrame(width, height, bad, ui, renderer);
        if (render_result != 0)
            return render_result;

        if (!render_frame::ImagesMatch(bad.c_str(), good.c_str()))
        {
            SDL_Log("%s: image differs from golden reference", name.c_str());
            return 1;
        }
        SDL_Log("%s: image matches golden reference", name.c_str());
        return 0;
    }

} // namespace render_lib
