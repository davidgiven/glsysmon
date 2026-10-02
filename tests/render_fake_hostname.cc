// Visual-snapshot test: runs the app with only HostnameView enabled, using a
// fake HostnameSensor, captures a PNG of the result, and compares it pixel by
// pixel against a golden reference. The test passes when the two images are
// identical.

#include <memory>
#include <string>

#include "app.h"
#include "imguiif.h"
#include "display/imgui_frame_renderer.h"
#include "preferences/preferences.h"
#include "render_lib.h"
#include "sensors/hostname_sensor.h"
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

    class FakeHostnameSensor : public HostnameSensor
    {
    public:
        FakeHostnameSensor(): HostnameSensor("hostname") {}

        std::string GetHostname() override
        {
            return "fake-hostname";
        }

        std::string GetHumanName() const override
        {
            return "Hostname";
        }

        std::string GetPrefName() const override
        {
            return "hostname";
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

        std::unique_ptr<HostnameSensor> CreateHostnameSensor(
            const std::string& prefPrefix) const override
        {
            (void)prefPrefix;
            return std::make_unique<FakeHostnameSensor>();
        }
    };

} // namespace

int main()
{
    CliArgs args;
    args.values = {"--views=HostnameView"};
    std::shared_ptr<Preferences> prefs = render_lib::CreateTestPreferences(args);
    auto timer = CreateTimer();
    auto imgui = CreateImGui();
    TestApp app(prefs, *timer, *imgui);
    FakeSensors sensors(app);
    auto ui = CreateUi(app, sensors);
    auto renderer = CreateImGuiFrameRenderer(*prefs);
    return render_lib::Run("render_fake_hostname", *ui, *renderer);
}
