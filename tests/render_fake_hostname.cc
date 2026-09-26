// Visual-snapshot test: runs the app with only HostnameView enabled, using a
// fake HostnameSensor, captures a PNG of the result, and compares it pixel by
// pixel against a golden reference. The test passes when the two images are
// identical.

#include <memory>
#include <string>

#include "app.h"
#include "display/imgui_frame_renderer.h"
#include "preferences/preferences.h"
#include "render_lib.h"
#include "sensors/hostname_sensor.h"
#include "sensors/sensors.h"
#include "timer.h"
#include "ui.h"

namespace
{

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

    class FakeSensors : public Sensors
    {
    public:
        FakeSensors(const Preferences& prefs, Timer& timer):
            Sensors(prefs, timer)
        {
        }

        std::unique_ptr<HostnameSensor> CreateHostnameSensor(
            const std::string& prefPrefix) const override
        {
            (void)prefPrefix;
            return std::make_unique<FakeHostnameSensor>();
        }
    };

    class FakeApp : public App
    {
    public:
        void Setup() override {}

        void MainLoop() override {}

        void Shutdown() override {}

        std::shared_ptr<Preferences> GetPreferences() override
        {
            return nullptr;
        }

        void Quit() override {}
    };

} // namespace

int main()
{
    CliArgs args;
    args.values = {"--views=HostnameView"};
    auto prefs = CreatePreferences(args);
    auto timer = CreateTimer();
    FakeSensors sensors(*prefs, *timer);
    FakeApp app;
    auto ui = CreateUi(*prefs, sensors, app);
    auto renderer = CreateImGuiFrameRenderer();
    return render_lib::Run("render_fake_hostname", *ui, *renderer);
}
