// Visual-snapshot test: runs the app with only DiskView enabled, using a
// fake DiskSensor, captures a PNG of the result, and compares it pixel by
// pixel against a golden reference. The test passes when the two images are
// identical.

#include <memory>
#include <string>
#include <vector>

#include "app.h"
#include "imguiif.h"
#include "display/imgui_frame_renderer.h"
#include "preferences/preferences.h"
#include "render_lib.h"
#include "sensors/disk_sensor.h"
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


    class FakeDiskSensor : public DiskSensor
    {
    public:
        FakeDiskSensor(): DiskSensor("disk")
        {
            _samples.resize(2);
            _names = {"sda", "sdb"};
            for (int i = 0; i < 60; ++i)
            {
                const double rx0 = 200'000.0 + 600'000.0 * (i % 10) / 10.0;
                const double tx0 =
                    100'000.0 + 300'000.0 * ((9 - (i % 10)) / 10.0);
                const double rx1 = 800'000.0 - 600'000.0 * (i % 10) / 10.0;
                const double tx1 = 500'000.0 * (i % 10) / 10.0;
                _samples[0].push_back({tx0, rx0});
                _samples[1].push_back({tx1, rx1});
            }
        }

        std::size_t GetChannels() const override
        {
            return _samples.size();
        }

        std::size_t GetSampleCount() const override
        {
            if (_samples.empty())
                return 0;
            return _samples[0].size();
        }

        const DiskSample* GetSamples(std::size_t ch) const override
        {
            if (ch >= _samples.size())
                return nullptr;
            return _samples[ch].data();
        }

        std::string GetChannelName(std::size_t channel) const override
        {
            if (channel < _names.size())
                return _names[channel];
            return "sd" + std::to_string(channel);
        }

        std::string GetHumanName() const override
        {
            return "Disk";
        }

        std::string GetPrefName() const override
        {
            return "disk";
        }

        void DrawConfiguration(Preferences& preferences) override
        {
            (void)preferences;
        }

    private:
        std::vector<std::vector<DiskSample>> _samples;
        std::vector<std::string> _names;
    };

    class FakeSensors : public MockSensors
    {
    public:
        FakeSensors(App& app): MockSensors(app.GetTimer()) {}

        std::unique_ptr<DiskSensor> CreateDiskSensor(
            const std::string& prefPrefix,
            const std::string& procDiskStatsPath) const override
        {
            (void)prefPrefix;
            (void)procDiskStatsPath;
            return std::make_unique<FakeDiskSensor>();
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
    args.values = {
        "--views=DiskView", "--disk.devices=sda,sdb", "--disk.maximum=1000000"};
    std::shared_ptr<Preferences> prefs = render_lib::CreateTestPreferences(args);
    auto timer = CreateTimer();
    auto imgui = CreateImGui();
    TestApp app(prefs, *timer, *imgui);
    FakeSensors sensors(app);
    auto ui = CreateUi(app, sensors);
    auto renderer = CreateImGuiFrameRenderer(*prefs);
    return render_lib::Run("render_fake_disk", *ui, *renderer);
}
