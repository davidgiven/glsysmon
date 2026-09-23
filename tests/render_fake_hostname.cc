// Visual-snapshot test: runs the app with only HostnameView enabled, using a
// fake HostnameSensor, captures a PNG of the result, and compares it pixel by
// pixel against a golden reference. The test passes when the two images are
// identical.

#include <memory>
#include <string>

#include "app.h"
#include "components.h"
#include "render_lib.h"
#include "sensors/hostname_sensor.h"

namespace
{

    class FakeHostnameSensor : public HostnameSensor
    {
    public:
        std::string GetHostname() override
        {
            return "fake-hostname";
        }

        std::string GetName() const override
        {
            return "Hostname";
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
