// Visual-snapshot test: runs the app with only ClockView enabled, using a
// fake ClockSensor, captures a PNG of the result, and compares it pixel by
// pixel against a golden reference. The test passes when the two images are
// identical.

#include <ctime>
#include <memory>
#include <string>

#include "app.h"
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
    class TestApp : public App
    {
    public:
        TestApp(std::shared_ptr<Preferences> prefs, Timer& timer, ImGuiIf& imgui):
            _prefs(std::move(prefs)), _timer(timer), _imgui(imgui) {}
        void Setup() override {}
        void MainLoop() override {}
        void Shutdown() override {}
        std::shared_ptr<Preferences> GetSharedPreferences() override { return _prefs; }
        Preferences& GetPreferences() override { return *_prefs; }
        const Preferences& GetPreferences() const override { return *_prefs; }
        Timer& GetTimer() override { return _timer; }
        ImGuiIf& GetImGui() override { return _imgui; }
        const Timer& GetTimer() const override { return _timer; }
        const ImGuiIf& GetImGui() const override { return _imgui; }
        void Quit() override {}
    private:
        std::shared_ptr<Preferences> _prefs;
        Timer& _timer;
        ImGuiIf& _imgui;
    };


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
        FakeSensors(App& app): MockSensors(app.GetTimer()) {}

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

        std::shared_ptr<Preferences> GetSharedPreferences() override { return nullptr; }
        Preferences& GetPreferences() override { static auto p = CreateMapPreferences(); return *p; }
        const Preferences& GetPreferences() const override { static auto p = CreateMapPreferences(); return *p; }
        Timer& GetTimer() override { static auto t = CreateTimer(); return *t; }
        ImGuiIf& GetImGui() override { static auto i = CreateImGui(); return *i; }
        const Timer& GetTimer() const override { static auto t = CreateTimer(); return *t; }
        const ImGuiIf& GetImGui() const override { static auto i = CreateImGui(); return *i; }

        void Quit() override {}
    };

} // namespace

int main()
{
    CliArgs args;
    args.values = {"--views=ClockView"};
    std::shared_ptr<Preferences> prefs = render_lib::CreateTestPreferences(args);
    auto timer = CreateTimer();
    auto imgui = CreateImGui();
    TestApp app(prefs, *timer, *imgui);
    FakeSensors sensors(app);
    auto ui = CreateUi(app, sensors);
    auto renderer = CreateImGuiFrameRenderer(*prefs);
    return render_lib::Run("render_fake_clock", *ui, *renderer);
}
