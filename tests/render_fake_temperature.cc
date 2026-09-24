// Visual-snapshot test: runs the app with only TemperatureView enabled, using a
// fake TemperatureSensor, captures a PNG of the result, and compares it pixel
// by pixel against a golden reference. The test passes when the two images are
// identical.

#include <memory>
#include <string>
#include <vector>

#include "app.h"
#include "display/imgui_frame_renderer.h"
#include "preferences/preferences.h"
#include "render_lib.h"
#include "sensors/temperature_sensor.h"
#include "timer.h"
#include "ui.h"

namespace
{

    class FakeTemperatureSensor : public TemperatureSensor
    {
    public:
        FakeTemperatureSensor()
        {
            _samples.resize(2);
            for (int i = 0; i < 60; ++i)
            {
                const double v0 = 30.0 + 20.0 * (i % 10) / 10.0;
                const double v1 = 70.0 - 20.0 * (i % 10) / 10.0;
                _samples[0].push_back(v0);
                _samples[1].push_back(v1);
            }
            _names = {"Tctl", "temp1"};
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

        const double* GetSamples(std::size_t ch) const override
        {
            if (ch >= _samples.size())
                return nullptr;
            return _samples[ch].data();
        }

        std::string GetChannelName(std::size_t channel) const override
        {
            if (channel < _names.size())
                return _names[channel];
            return "temp" + std::to_string(channel + 1);
        }

        std::string GetName() const override
        {
            return "Temperature";
        }

        void DrawConfiguration() override {}

    private:
        std::vector<std::vector<double>> _samples;
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
        "--views=TemperatureView", "--temperature.sensors=Tctl,temp1"};
    auto prefs = CreatePreferences(args);
    auto timer = CreateTimer();
    auto fakeSensor = std::make_unique<FakeTemperatureSensor>();
    FakeApp app;
    auto ui =
        CreateUiWithFakeTemperature(*prefs, *timer, std::move(fakeSensor), app);
    auto renderer = CreateImGuiFrameRenderer();
    return render_lib::Run("render_fake_temperature", *ui, *renderer);
}
