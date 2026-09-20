// Visual-snapshot test: runs the app with only ClockView enabled, using a
// fake ClockSensor, captures a PNG of the result, and compares it pixel by
// pixel against a golden reference. The test passes when the two images are
// identical.

#include <ctime>
#include <memory>

#include "components.h"
#include "imgui_frame_renderer.h"
#include "render_frame.h"
#include "sensors/clock_sensor.h"
#include "ui.h"

namespace
{

    class FakeClockSensor : public ClockSensor
    {
    public:
        std::tm GetLocalTime() override
        {
            std::tm tm{};
            tm.tm_year = 2020 - 1900;
            tm.tm_mon = 0;
            tm.tm_mday = 2;
            tm.tm_hour = 3;
            tm.tm_min = 4;
            tm.tm_sec = 5;
            return tm;
        }
    };

} // namespace

int main()
{
    CliArgs args;
    args.values = {"--views=ClockView"};
    auto prefs = CreatePreferences(args);
    auto fakeSensor = std::make_unique<FakeClockSensor>();
    auto ui = CreateUiWithFakeClock(*prefs, std::move(fakeSensor));
    auto renderer = CreateImGuiFrameRenderer();
    const int render_result = render_frame::RenderFrame(
        240, 0, "tests/render_fake_clock.bad.png", *ui, *renderer);
    if (render_result != 0)
        return render_result;

    if (!render_frame::ImagesMatch("tests/render_fake_clock.bad.png",
            "tests/render_fake_clock.good.png"))
    {
        SDL_Log("render_fake_clock: image differs from golden reference");
        return 1;
    }
    SDL_Log("render_fake_clock: image matches golden reference");
    return 0;
}
