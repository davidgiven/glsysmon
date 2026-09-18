// Visual-snapshot test: runs the app with only HostnameView enabled, using a
// fake HostnameSensor, captures a PNG of the result, and compares it pixel by
// pixel against a golden reference. The test passes when the two images are
// identical.

#include <memory>
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
        std::string GetHostname() override
        {
            return "fake-hostname";
        }
    };

} // namespace

int main()
{
    CliArgs args;
    args.values = {"--views=HostnameView"};
    auto prefs = CreatePreferences(args);
    auto fakeSensor = std::make_unique<FakeHostnameSensor>();
    auto ui = CreateUiWithFakeHostname(*prefs, std::move(fakeSensor));
    auto renderer = CreateImGuiFrameRenderer();
    const int render_result = render_frame::RenderFrame(
        240, 0, "tests/render_fake_hostname.bad.png", *ui, *renderer);
    if (render_result != 0)
        return render_result;

    if (!render_frame::ImagesMatch("tests/render_fake_hostname.bad.png",
            "tests/render_fake_hostname.good.png"))
    {
        SDL_Log("render_fake_hostname: image differs from golden reference");
        return 1;
    }
    SDL_Log("render_fake_hostname: image matches golden reference");
    return 0;
}
