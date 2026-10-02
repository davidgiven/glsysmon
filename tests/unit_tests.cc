#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <cstdio>
#include <fstream>
#include <memory>
#include <string>
#include <vector>

#include "app.h"
#include "display/imgui_frame_renderer.h"
#include "imguiif.h"
#include "preferences/preferences.h"
#include "sensors/cpu_sensor.h"
#include "sensors/hostname_sensor.h"
#include "mock_sensors.h"
#include "timer.h"
#include "ui.h"
#include "views/view.h"
#include "views/views.h"

namespace
{

    class TestApp : public App
    {
    public:
        TestApp(std::shared_ptr<Preferences> prefs, Timer& timer, ImGuiIf& imgui):
            _prefs(std::move(prefs)),
            _timer(timer),
            _imgui(imgui)
        {
        }
        void Setup() override {}
        void MainLoop() override {}
        void Shutdown() override {}
        std::shared_ptr<Preferences> GetSharedPreferences() override
        {
            return _prefs;
        }
        Preferences& GetPreferences() override
        {
            return *_prefs;
        }
        const Preferences& GetPreferences() const override
        {
            return *_prefs;
        }
        Timer& GetTimer() override
        {
            return _timer;
        }
        ImGuiIf& GetImGui() override
        {
            return _imgui;
        }
        const Timer& GetTimer() const override
        {
            return _timer;
        }
        const ImGuiIf& GetImGui() const override
        {
            return _imgui;
        }
        void Quit() override {}
    private:
        std::shared_ptr<Preferences> _prefs;
        Timer& _timer;
        ImGuiIf& _imgui;
    };

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

    class TestSensors : public MockSensors
    {
    public:
        explicit TestSensors(App& app):
            MockSensors(app.GetTimer()),
            _app(app)
        {
        }

        std::unique_ptr<CpuSensor> CreateCpuSensor(
            const std::string& prefPrefix,
            const std::string& procStatPath) const override
        {
            return ::CreateCpuSensor(
                _app, prefPrefix, ::CreateCpuPoller(procStatPath));
        }

        std::unique_ptr<HostnameSensor> CreateHostnameSensor(
            const std::string& prefPrefix) const override
        {
            (void)prefPrefix;
            return std::make_unique<FakeHostnameSensor>();
        }

    private:
        App& _app;
    };

} // namespace

TEST_CASE("CreateUi creates a UI component")
{
    CliArgs args;
    args.values = {"--views=HostnameView"};
    std::shared_ptr<Preferences> prefs = CreatePreferences(args);
    auto timer = CreateTimer();
    auto imgui = CreateImGui();
    TestApp app(prefs, *timer, *imgui);
    TestSensors sensors(app);
    auto ui = CreateUi(app, sensors);
    CHECK(ui != nullptr);
}

TEST_CASE("CreateHostnameView with a fake sensor creates a view")
{
    auto fake = std::make_unique<FakeHostnameSensor>();
    CHECK(fake->GetHostname() == "fake-hostname");

    CliArgs args;
    std::shared_ptr<Preferences> prefs = CreatePreferences(args);
    auto timer = CreateTimer();
    auto imgui = CreateImGui();
    TestApp app(prefs, *timer, *imgui);
    auto view = CreateHostnameView(app, std::move(fake));
    CHECK(view != nullptr);
}

TEST_CASE("CreateHostnameView creates a view component")
{
    CliArgs args;
    std::shared_ptr<Preferences> prefs = CreatePreferences(args);
    auto timer = CreateTimer();
    auto imgui = CreateImGui();
    TestApp app(prefs, *timer, *imgui);
    TestSensors sensors(app);
    auto view = CreateHostnameView(app, sensors);
    CHECK(view != nullptr);
}

TEST_CASE("View catalogue exposes HostnameView and resolves it")
{
    CliArgs args;
    std::shared_ptr<Preferences> prefs = CreatePreferences(args);
    auto timer = CreateTimer();
    auto imgui = CreateImGui();
    TestApp app(prefs, *timer, *imgui);
    TestSensors sensors(app);
    Views views(app, sensors);
    View* view = views.Get("HostnameView");
    REQUIRE(view != nullptr);
    CHECK(views.Get("UnknownView") == nullptr);
    View* view2 = views.Get("HostnameView");
    CHECK(view == view2);
}

TEST_CASE("Hostname sensor returns the current hostname")
{
    CliArgs args;
    std::shared_ptr<Preferences> prefs = CreatePreferences(args);
    auto timer = CreateTimer();
    auto imgui = CreateImGui();
    TestApp app(prefs, *timer, *imgui);
    auto sensor = CreateHostnameSensor(app, "hostname");

    const std::string hostname = sensor->GetHostname();
    CHECK_FALSE(hostname.empty());
    CHECK(hostname.size() < 256);
}

TEST_CASE("CreateImGuiFrameRenderer creates a frame renderer")
{
    auto renderer = CreateImGuiFrameRenderer();
    CHECK(renderer != nullptr);
}

TEST_CASE("CLI preferences default to left / 100 / monitor 0")
{
    CliArgs args;
    auto prefs = CreateCliPreferences(args);

    CHECK(GlobalPreferencesFetcher::GetSide(*prefs) == "left");
    CHECK(GlobalPreferencesFetcher::GetSize(*prefs) == 100);
    CHECK(GlobalPreferencesFetcher::GetMonitor(*prefs) == 0);
    CHECK(GlobalPreferencesFetcher::GetViews(*prefs) ==
          std::vector<std::string>{"HostnameView",
              "ClockView",
              "CpuView",
              "TemperatureView",
              "NetworkView",
              "DiskView",
              "MemoryView"});
}

TEST_CASE("CLI --views= parses a comma-separated list")
{
    CliArgs args;
    args.values = {"--views=HostnameView,CpuView"};
    auto prefs = CreateCliPreferences(args);

    CHECK(GlobalPreferencesFetcher::GetViews(*prefs) ==
          std::vector<std::string>{"HostnameView", "CpuView"});
}

TEST_CASE("CLI preferences parse --side= / --size= / --monitor=")
{
    CliArgs args;
    args.values = {"--side=right", "--size=120", "--monitor=2"};
    auto prefs = CreateCliPreferences(args);

    CHECK(GlobalPreferencesFetcher::GetSide(*prefs) == "right");
    CHECK(GlobalPreferencesFetcher::GetSize(*prefs) == 120);
    CHECK(GlobalPreferencesFetcher::GetMonitor(*prefs) == 2);
}

TEST_CASE("CLI preference values that are not integers fall back to defaults")
{
    CliArgs args;
    args.values = {"--size=abc"};
    auto prefs = CreateCliPreferences(args);

    CHECK(prefs->GetInteger("size") == std::nullopt);
    CHECK(GlobalPreferencesFetcher::GetSize(*prefs) == 100);
}

TEST_CASE("CpuSensorImpl reads dummy proc file")
{
    CliArgs args;
    std::shared_ptr<Preferences> prefs = CreatePreferences(args);
    const std::string path = ".obj/test_cpu_stat_dummy";
    {
        std::ofstream out(path);
        out << "cpu  100 0 100 1000 0 0 0 0 0 0\n";
        out << "cpu0 50 0 50 500 0 0 0 0 0 0\n";
        out << "cpu1 50 0 50 500 0 0 0 0 0 0\n";
    }

    auto timer = CreateTimer();
    auto imgui = CreateImGui();
    TestApp app(prefs, *timer, *imgui);
    auto sensor = CreateCpuSensor(app, "cpu", CreateCpuPoller(path));
    const int cpuInterval =
        prefs->GetInteger("cpu.update_interval").value_or(5);
    uint64_t delta = 1'000'000'000ULL / static_cast<uint64_t>(cpuInterval);
    if (delta == 0)
        delta = 100'000'000ULL;
    REQUIRE(sensor != nullptr);
    CHECK(sensor->GetChannels() == 2);
    const std::size_t expectedSamples =
        static_cast<std::size_t>(GlobalPreferencesFetcher::GetSize(*prefs));
    CHECK(sensor->GetSampleCount() == expectedSamples);
    // Initially all samples are zeros (fixed-size buffer)
    {
        const CpuSample* s0_init = sensor->GetSamples(0);
        REQUIRE(s0_init != nullptr);
        for (std::size_t i = 0; i < expectedSamples; ++i)
        {
            CHECK(s0_init[i].user == doctest::Approx(0.0f));
            CHECK(s0_init[i].system == doctest::Approx(0.0f));
            CHECK(s0_init[i].nice == doctest::Approx(0.0f));
        }
    }

    timer->Tick(timer->Now() + delta);
    REQUIRE(sensor->GetSampleCount() == expectedSamples);
    const CpuSample* s0 = sensor->GetSamples(0);
    REQUIRE(s0 != nullptr);
    CHECK(s0[expectedSamples - 1].user == doctest::Approx(0.0f));
    CHECK(s0[expectedSamples - 1].system == doctest::Approx(0.0f));
    CHECK(s0[expectedSamples - 1].nice == doctest::Approx(0.0f));

    {
        std::ofstream out(path);
        out << "cpu  200 0 200 1200 0 0 0 0 0 0\n";
        out << "cpu0 100 0 100 600 0 0 0 0 0 0\n";
        out << "cpu1 100 0 100 600 0 0 0 0 0 0\n";
    }

    timer->Tick(timer->Now() + delta);
    REQUIRE(sensor->GetSampleCount() == expectedSamples);
    s0 = sensor->GetSamples(0);
    REQUIRE(s0 != nullptr);
    CHECK(s0[expectedSamples - 1].user == doctest::Approx(0.25f));
    CHECK(s0[expectedSamples - 1].system == doctest::Approx(0.25f));
    CHECK(s0[expectedSamples - 1].nice == doctest::Approx(0.0f));
    // Previous sample was zero (shifted)
    CHECK(s0[expectedSamples - 2].user == doctest::Approx(0.0f));

    // Verify const overloads and span accessor
    const CpuSensor& cs = *sensor;
    CHECK(cs.GetChannels() == 2);
    CHECK(cs.GetSampleCount() == expectedSamples);
    const CpuSample* cs0 = cs.GetSamples(0);
    REQUIRE(cs0 != nullptr);
    CHECK(cs0[expectedSamples - 1].user == doctest::Approx(0.25f));
    auto span = cs.GetSamplesSpan(0);
    REQUIRE(span.size() == expectedSamples);
    CHECK(span[expectedSamples - 1].user == doctest::Approx(0.25f));

    // Values in [0,1]
    for (std::size_t cpu = 0; cpu < sensor->GetChannels(); ++cpu)
    {
        const CpuSample* s = sensor->GetSamples(cpu);
        REQUIRE(s != nullptr);
        for (std::size_t i = 0; i < sensor->GetSampleCount(); ++i)
        {
            CHECK(s[i].user >= 0.0f);
            CHECK(s[i].user <= 1.0f);
            CHECK(s[i].system >= 0.0f);
            CHECK(s[i].system <= 1.0f);
            CHECK(s[i].nice >= 0.0f);
            CHECK(s[i].nice <= 1.0f);
        }
    }

    // Via Sensors factory
    TestSensors sensors(app);
    auto factorySensor = sensors.CreateCpuSensor("cpu", path);
    CHECK(factorySensor->GetChannels() == 2);
    // Via view-owned sensor
    auto view = CreateCpuView(app, sensors.CreateCpuSensor("cpu", path));
    REQUIRE(view != nullptr);
    CHECK(view->GetSensors().size() == 1);
    auto* viewSensor = dynamic_cast<CpuSensor*>(view->GetSensors()[0]);
    REQUIRE(viewSensor != nullptr);
    CHECK(viewSensor->GetChannels() == 2);

    std::remove(path.c_str());
}

TEST_CASE("CpuSensorImpl handles missing file gracefully")
{
    CliArgs args;
    std::shared_ptr<Preferences> prefs = CreatePreferences(args);
    const std::string missing = ".obj/nonexistent_cpu_stat";
    std::remove(missing.c_str());
    auto timer = CreateTimer();
    auto imgui2 = CreateImGui();
    TestApp app(prefs, *timer, *imgui2);
    auto sensor = CreateCpuSensor(app, "cpu", CreateCpuPoller(missing));
    CHECK(sensor->GetChannels() == 0);
    CHECK(sensor->GetSampleCount() == 0);
    const int cpuInterval =
        prefs->GetInteger("cpu.update_interval").value_or(5);
    uint64_t delta = 1'000'000'000ULL / static_cast<uint64_t>(cpuInterval);
    if (delta == 0)
        delta = 100'000'000ULL;
    timer->Tick(timer->Now() + delta);
    CHECK(sensor->GetSampleCount() == 0);
    CHECK(sensor->GetSamples(0) == nullptr);
}
