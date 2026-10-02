// Visual-snapshot test: runs the app with only CpuView enabled, using a
// fake CpuSensor, captures a PNG of the result, and compares it pixel by
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
#include "sensors/cpu_sensor.h"
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


    class FakeCpuSensor : public CpuSensor
    {
    public:
        FakeCpuSensor(): CpuSensor("cpu")
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

        std::string GetHumanName() const override
        {
            return "Cpu";
        }

        std::string GetPrefName() const override
        {
            return "cpu";
        }

        void DrawConfiguration(Preferences& preferences) override
        {
            (void)preferences;
        }

    private:
        std::vector<std::vector<CpuSample>> _samples;
    };

    class FakeSensors : public MockSensors
    {
    public:
        FakeSensors(App& app): MockSensors(app.GetTimer()) {}

        std::unique_ptr<CpuSensor> CreateCpuSensor(
            const std::string& prefPrefix,
            const std::string& procStatPath) const override
        {
            (void)prefPrefix;
            (void)procStatPath;
            return std::make_unique<FakeCpuSensor>();
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
    args.values = {"--views=CpuView"};
    std::shared_ptr<Preferences> prefs = render_lib::CreateTestPreferences(args);
    auto timer = CreateTimer();
    auto imgui = CreateImGui();
    TestApp app(prefs, *timer, *imgui);
    FakeSensors sensors(app);
    auto ui = CreateUi(app, sensors);
    auto renderer = CreateImGuiFrameRenderer(*prefs);
    return render_lib::Run("render_fake_cpu", *ui, *renderer);
}
