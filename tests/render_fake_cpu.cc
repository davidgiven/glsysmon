// Visual-snapshot test: runs the app with only CpuView enabled, using a
// fake CpuSensor, captures a PNG of the result, and compares it pixel by
// pixel against a golden reference. The test passes when the two images are
// identical.

#include <memory>
#include <vector>

#include "components.h"
#include "display/imgui_frame_renderer.h"
#include "render_frame.h"
#include "sensors/cpu_sensor.h"
#include "ui.h"

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

        std::size_t GetCpuCount() override
        {
            return _samples.size();
        }

        std::size_t GetSampleCount() override
        {
            if (_samples.empty())
                return 0;
            return _samples[0].size();
        }

        const CpuSample* GetSamples(std::size_t cpu) override
        {
            if (cpu >= _samples.size())
                return nullptr;
            return _samples[cpu].data();
        }

        void Tick() override {}

    private:
        std::vector<std::vector<CpuSample>> _samples;
    };

} // namespace

int main()
{
    CliArgs args;
    args.values = {"--views=CpuView"};
    auto prefs = CreatePreferences(args);
    auto fakeSensor = std::make_unique<FakeCpuSensor>();
    auto ui = CreateUiWithFakeCpu(*prefs, std::move(fakeSensor));
    auto renderer = CreateImGuiFrameRenderer();
    const int render_result = render_frame::RenderFrame(
        240, 0, "tests/render_fake_cpu.bad.png", *ui, *renderer);
    if (render_result != 0)
        return render_result;

    if (!render_frame::ImagesMatch(
            "tests/render_fake_cpu.bad.png", "tests/render_fake_cpu.good.png"))
    {
        SDL_Log("render_fake_cpu: image differs from golden reference");
        return 1;
    }
    SDL_Log("render_fake_cpu: image matches golden reference");
    return 0;
}
