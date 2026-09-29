// Visual-snapshot test: runs the app with only HostnameView enabled, using a
// fake HostnameSensor, captures a PNG of the result, and compares it pixel by
// pixel against a golden reference. The test passes when the two images are
// identical.

#include <memory>
#include <string>

#include "app.h"
#include "context.h"
#include "imguiif.h"
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
        FakeSensors(const Context& ctx):
            Sensors(ctx)
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
            static Context dummyCtx(*this, *dummyImgui, *dummyPrefs, *dummyTimer);
            return dummyCtx;
        }

        void Quit() override {}
    };

} // namespace

int main()
{
    CliArgs args;
    args.values = {"--views=HostnameView"};
    auto prefs = render_lib::CreateTestPreferences(args);
    auto timer = CreateTimer();
    auto imgui = CreateImGui();
    FakeApp app;
    Context ctx(app, *imgui, *prefs, *timer);
    FakeSensors sensors(ctx);
    auto ui = CreateUi(ctx, sensors);
    auto renderer = CreateImGuiFrameRenderer();
    return render_lib::Run("render_fake_hostname", *ui, *renderer);
}
