// Visual-snapshot test: runs the app with only ClockView enabled, using a
// fake ClockSensor, captures a PNG of the result, and compares it pixel by
// pixel against a golden reference. The test passes when the two images are
// identical.

#include <ctime>
#include <memory>
#include <string>

#include "app.h"
#include "components.h"
#include "render_lib.h"
#include "sensors/clock_sensor.h"

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

        std::string GetName() const override
        {
            return "Clock";
        }

        void DrawConfiguration() override {}
    };

    class FakeApp : public App
    {
    public:
        void Setup() override {}

        void MainLoop() override {}

        void Shutdown() override {}

        void Quit() override {}
    };

} // namespace

int main()
{
    CliArgs args;
    args.values = {"--views=ClockView"};
    auto prefs = CreatePreferences(args);
    auto timer = CreateTimer();
    auto fakeSensor = std::make_unique<FakeClockSensor>();
    FakeApp app;
    auto ui = CreateUiWithFakeClock(*prefs, *timer, std::move(fakeSensor), app);
    auto renderer = CreateImGuiFrameRenderer();
    return render_lib::Run("render_fake_clock", *ui, *renderer);
}
