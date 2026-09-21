// Visual-snapshot test: runs the app with only CpuView enabled, using a
// fake CpuSensor, captures a PNG of the result, and compares it pixel by
// pixel against a golden reference. The test passes when the two images are
// identical.

#include <memory>
#include <vector>

#include "components.h"
#include "render_lib.h"
#include "sensors/cpu_sensor.h"

namespace
{

    class FakeCpuSensor : public CpuSensor
    {
    public:
        FakeCpuSensor()
        {
            _samples.resize(2);
            for (int i = 0; i < 60; ++i)
            {
                const float v0 = 0.2f + 0.5f * (i % 10) / 10.0f;
                const float v1 = 0.8f - 0.5f * (i % 10) / 10.0f;
                _samples[0].push_back({v0, 0.1f, 0.0f});
                _samples[1].push_back({v1, 0.1f, 0.0f});
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

        const CpuSample* GetSamples(std::size_t cpu) const override
        {
            if (cpu >= _samples.size())
                return nullptr;
            return _samples[cpu].data();
        }

        std::string GetChannelName(std::size_t channel) const override
        {
            return "CPU" + std::to_string(channel);
        }

    private:
        std::vector<std::vector<CpuSample>> _samples;
    };

} // namespace

int main()
{
    CliArgs args;
    args.values = {"--views=CpuView"};
    auto prefs = CreatePreferences(args);
    auto timer = CreateTimer();
    auto fakeSensor = std::make_unique<FakeCpuSensor>();
    auto ui = CreateUiWithFakeCpu(*prefs, *timer, std::move(fakeSensor));
    auto renderer = CreateImGuiFrameRenderer();
    return render_lib::Run("render_fake_cpu", *ui, *renderer);
}
