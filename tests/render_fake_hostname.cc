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
    auto fakeSensor = std::make_unique<FakeHostnameSensor>();
    FakeApp app;
    auto ui =
        CreateUiWithFakeHostname(*prefs, *timer, std::move(fakeSensor), app);
    auto renderer = CreateImGuiFrameRenderer();
    return render_lib::Run("render_fake_hostname", *ui, *renderer);
}
