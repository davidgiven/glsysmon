// Visual-snapshot test: runs the app with only NetworkView enabled, using a
// fake NetworkSensor, captures a PNG of the result, and compares it pixel by
// pixel against a golden reference. The test passes when the two images are
// identical.

#include <memory>
#include <string>
#include <vector>

#include "app.h"
#include "display/imgui_frame_renderer.h"
#include "preferences/preferences.h"
#include "render_lib.h"
#include "sensors/network_sensor.h"
#include "sensors/sensors.h"
#include "timer.h"
#include "ui.h"

namespace
{

    class FakeNetworkSensor : public NetworkSensor
    {
    public:
        FakeNetworkSensor(): NetworkSensor("network")
        {
            _samples.resize(2);
            _names = {"eth0", "wlan0"};
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

        const NetworkSample* GetSamples(std::size_t ch) const override
        {
            if (ch >= _samples.size())
                return nullptr;
            return _samples[ch].data();
        }

        std::string GetChannelName(std::size_t channel) const override
        {
            if (channel < _names.size())
                return _names[channel];
            return "eth" + std::to_string(channel);
        }

        std::string GetHumanName() const override
        {
            return "Network";
        }

        std::string GetPrefName() const override
        {
            return "network";
        }

        void DrawConfiguration(Preferences& preferences) override
        {
            (void)preferences;
        }

    private:
        std::vector<std::vector<NetworkSample>> _samples;
        std::vector<std::string> _names;
    };

    class FakeSensors : public Sensors
    {
    public:
        FakeSensors(const Preferences& prefs, Timer& timer):
            Sensors(prefs, timer)
        {
        }

        std::unique_ptr<NetworkSensor> CreateNetworkSensor(
            const std::string& prefPrefix,
            const std::string& procNetDevPath) const override
        {
            (void)prefPrefix;
            (void)procNetDevPath;
            return std::make_unique<FakeNetworkSensor>();
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
    args.values = {"--views=NetworkView",
        "--network.interfaces=eth0,wlan0",
        "--network.maximum=1000000"};
    auto prefs = CreatePreferences(args);
    auto timer = CreateTimer();
    FakeSensors sensors(*prefs, *timer);
    FakeApp app;
    auto ui = CreateUi(*prefs, sensors, app);
    auto renderer = CreateImGuiFrameRenderer();
    return render_lib::Run("render_fake_network", *ui, *renderer);
}
