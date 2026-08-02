// Visual-snapshot test: runs the app with only HostnameView enabled, using a
// fake HostnameSensor, captures a PNG of the result, and compares it pixel by
// pixel against a golden reference. The test passes when the two images are
// identical.

#include <SDL3/SDL.h>
#include <fruit/fruit.h>
#include <stb_image.h>

#include <cstring>
#include <string>

#include "components.h"
#include "imgui_frame_renderer.h"
#include "render_frame.h"
#include "sensors/hostname_sensor.h"
#include "ui.h"

namespace
{

    class FakeHostnameSensor : public HostnameSensor
    {
    public:
        using Inject = FakeHostnameSensor();

        std::string GetHostname() override
        {
            return "fake-hostname";
        }
    };

    fruit::Component<Ui, ImGuiFrameRenderer> GetFakeComponents(CliArgs* args)
    {
        return fruit::createComponent()
            .install(GetUiComponent)
            .install(GetImGuiFrameRendererComponent)
            .bindInstance(*args)
            .bind<HostnameSensor, FakeHostnameSensor>();
    }

    // Returns true if the two PNGs are pixel-identical (dimensions and bytes).
    bool ImagesMatch(const char* actual, const char* expected)
    {
        int actual_width = 0;
        int actual_height = 0;
        int expected_width = 0;
        int expected_height = 0;
        stbi_uc* actual_pixels =
            stbi_load(actual, &actual_width, &actual_height, nullptr, 4);
        stbi_uc* expected_pixels =
            stbi_load(expected, &expected_width, &expected_height, nullptr, 4);
        if (actual_pixels == nullptr || expected_pixels == nullptr)
        {
            SDL_Log("failed to load %s or %s", actual, expected);
            stbi_image_free(actual_pixels);
            stbi_image_free(expected_pixels);
            return false;
        }
        const bool match = actual_width == expected_width &&
                           actual_height == expected_height &&
                           std::memcmp(actual_pixels,
                               expected_pixels,
                               static_cast<std::size_t>(actual_width) *
                                   actual_height * 4) == 0;
        stbi_image_free(actual_pixels);
        stbi_image_free(expected_pixels);
        return match;
    }

} // namespace

int main()
{
    CliArgs args;
    args.values = {"--views=HostnameView"};
    const int render_result = render_frame::RenderFrame(
        240, 0, "tests/render_fake_hostname.bad.png", GetFakeComponents, &args);
    if (render_result != 0)
        return render_result;

    if (!ImagesMatch("tests/render_fake_hostname.bad.png",
            "tests/render_fake_hostname.good.png"))
    {
        SDL_Log("render_fake_hostname: image differs from golden reference");
        return 1;
    }
    SDL_Log("render_fake_hostname: image matches golden reference");
    return 0;
}
