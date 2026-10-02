// Visual-snapshot test: runs the app with only ClockView enabled, using a
// fake ClockSensor, captures a PNG of the result, and compares it pixel by
// pixel against a golden reference. The test passes when the two images are
// identical.

#include <ctime>
#include <memory>
#include <string>

#include "app.h"
#include "context.h"
#include "imguiif.h"
#include "display/imgui_frame_renderer.h"
#include "preferences/preferences.h"
#include "render_lib.h"
#include "sensors/clock_sensor.h"
#include "mock_sensors.h"
#include "timer.h"
#include "ui.h"

namespace
{

    class FakeClockSensor : public ClockSensor
    {
    public:
        FakeClockSensor(): ClockSensor("clock") {}

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

        std::string GetHumanName() const override
        {
            return "Clock";
        }

        std::string GetPrefName() const override
        {
            return "clock";
        }

        void DrawConfiguration(Preferences& preferences) override
        {
            (void)preferences;
        }
    };

    class FakeSensors : public MockSensors
    {
    public:
        FakeSensors(const Context& ctx): MockSensors(ctx.timer) {}

        std::unique_ptr<ClockSensor> CreateClockSensor(
            const std::string& prefPrefix) const override
        {
            (void)prefPrefix;
            return std::make_unique<FakeClockSensor>();
        }
    };

    class FakeApp : public App
    {
    public:
        FakeApp() {}

        void Setup() override {}

        void MainLoop() override {}

        void Shutdown() override {}

        std::shared_ptr<Preferences> GetPreferences() override
        {
            return nullptr;
        }

        Context& GetContext() override
        {
            static std::shared_ptr<Preferences> dummyPrefs =
                CreateMapPreferences();
            static auto dummyTimer = CreateTimer();
            static auto dummyImgui = CreateImGui();
            static Context dummyCtx(
                *this, *dummyImgui, *dummyPrefs, *dummyTimer);
            return dummyCtx;
        }

        void Quit() override {}
    };

} // namespace

int main()
{
    CliArgs args;
    args.values = {"--views=ClockView"};
    auto prefs = render_lib::CreateTestPreferences(args);
    auto timer = CreateTimer();
    auto imgui = CreateImGui();
    FakeApp app;
    Context ctx(app, *imgui, *prefs, *timer);
    FakeSensors sensors(ctx);
    auto ui = CreateUi(ctx, sensors);
    auto renderer = CreateImGuiFrameRenderer(*prefs);
    return render_lib::Run("render_fake_clock", *ui, *renderer);
}
