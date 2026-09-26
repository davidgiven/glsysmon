// Visual-snapshot test: runs the app with only DiskView enabled, using a
// fake DiskSensor, captures a PNG of the result, and compares it pixel by
// pixel against a golden reference. The test passes when the two images are
// identical.

#include <memory>
#include <string>
#include <vector>

#include "app.h"
#include "display/imgui_frame_renderer.h"
#include "preferences/preferences.h"
#include "render_lib.h"
#include "sensors/disk_sensor.h"
#include "timer.h"
#include "ui.h"

namespace
{

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
    args.values = {
        "--views=DiskView", "--disk.devices=sda,sdb", "--disk.maximum=1000000"};
    auto prefs = CreatePreferences(args);
    auto timer = CreateTimer();
    auto fakeSensor = std::make_unique<FakeDiskSensor>();
    FakeApp app;
    auto ui = CreateUiWithFakeDisk(*prefs, *timer, std::move(fakeSensor), app);
    auto renderer = CreateImGuiFrameRenderer();
    return render_lib::Run("render_fake_disk", *ui, *renderer);
}
