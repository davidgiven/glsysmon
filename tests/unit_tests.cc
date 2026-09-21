#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <cstdio>
#include <fstream>
#include <memory>
#include <string>
#include <vector>

#include "components.h"
#include "sensors/cpu_sensor.h"
#include "sensors/sensors.h"
#include "views/catalogue.h"

namespace
{

    class FakeHostnameSensor : public HostnameSensor
    {
    public:
        void Tick() override {}

        std::string GetHostname() override
        {
            return "fake-hostname";
        }
    };

} // namespace

TEST_CASE("CreateUi creates a UI component")
{
    CliArgs args;
    auto prefs = CreatePreferences(args);
    auto ui = CreateUi(*prefs);
    CHECK(ui != nullptr);
}

TEST_CASE("CreateHostnameView with a fake sensor creates a view")
{
    auto fake = std::make_unique<FakeHostnameSensor>();
    CHECK(fake->GetHostname() == "fake-hostname");

    CliArgs args;
    auto prefs = CreatePreferences(args);
    auto view = CreateHostnameView(*prefs, std::move(fake));
    CHECK(view != nullptr);
}

TEST_CASE("CreateHostnameView creates a view component")
{
    CliArgs args;
    auto prefs = CreatePreferences(args);
    Sensors sensors(*prefs);
    auto view = CreateHostnameView(*prefs, sensors);
    CHECK(view != nullptr);
}

TEST_CASE("View catalogue exposes HostnameView and resolves it")
{
    const auto& catalogue = GetViewCatalogue();
    REQUIRE(catalogue.find("HostnameView") != catalogue.end());

    CliArgs args;
    auto prefs = CreatePreferences(args);
    Sensors sensors(*prefs);
    auto view = catalogue.at("HostnameView")(*prefs, sensors);
    CHECK(view != nullptr);
}

TEST_CASE("Hostname sensor returns the current hostname")
{
    CliArgs args;
    auto prefs = CreatePreferences(args);
    auto sensor = CreateHostnameSensor(*prefs);

    const std::string hostname = sensor->GetHostname();
    CHECK_FALSE(hostname.empty());
    CHECK(hostname.size() < 256);
}

TEST_CASE("CreateImGuiFrameRenderer creates a frame renderer")
{
    auto renderer = CreateImGuiFrameRenderer();
    CHECK(renderer != nullptr);
}

TEST_CASE("CLI preferences default to left / 240 / monitor 0")
{
    CliArgs args;
    auto prefs = CreateCliPreferences(args);

    CHECK(GlobalPreferencesFetcher::GetSide(*prefs) == "left");
    CHECK(GlobalPreferencesFetcher::GetSize(*prefs) == 240);
    CHECK(GlobalPreferencesFetcher::GetMonitor(*prefs) == 0);
    CHECK(GlobalPreferencesFetcher::GetViews(*prefs) ==
          std::vector<std::string>{"HostnameView", "ClockView", "CpuView"});
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
    CHECK(GlobalPreferencesFetcher::GetSize(*prefs) == 240);
}

TEST_CASE("CpuSensorImpl reads dummy proc file")
{
    CliArgs args;
    auto prefs = CreatePreferences(args);
    const std::string path = ".obj/test_cpu_stat_dummy";
    {
        std::ofstream out(path);
        out << "cpu  100 0 100 1000 0 0 0 0 0 0\n";
        out << "cpu0 50 0 50 500 0 0 0 0 0 0\n";
        out << "cpu1 50 0 50 500 0 0 0 0 0 0\n";
    }

    auto sensor = CreateCpuSensor(*prefs, path);
    REQUIRE(sensor != nullptr);
    CHECK(sensor->GetCpuCount() == 2);
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

    sensor->Tick();
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

    sensor->Tick();
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
    CHECK(cs.GetCpuCount() == 2);
    CHECK(cs.GetSampleCount() == expectedSamples);
    const CpuSample* cs0 = cs.GetSamples(0);
    REQUIRE(cs0 != nullptr);
    CHECK(cs0[expectedSamples - 1].user == doctest::Approx(0.25f));
    auto span = cs.GetSamplesSpan(0);
    REQUIRE(span.size() == expectedSamples);
    CHECK(span[expectedSamples - 1].user == doctest::Approx(0.25f));

    // Values in [0,1]
    for (std::size_t cpu = 0; cpu < sensor->GetCpuCount(); ++cpu)
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

    // Via Sensors registry
    Sensors sensors(*prefs);
    sensors.SetCpuSensor(CreateCpuSensor(*prefs, path));
    CHECK(sensors.GetCpuSensor().GetCpuCount() == 2);

    std::remove(path.c_str());
}

TEST_CASE("CpuSensorImpl handles missing file gracefully")
{
    CliArgs args;
    auto prefs = CreatePreferences(args);
    const std::string missing = ".obj/nonexistent_cpu_stat";
    std::remove(missing.c_str());
    auto sensor = CreateCpuSensor(*prefs, missing);
    CHECK(sensor->GetCpuCount() == 0);
    CHECK(sensor->GetSampleCount() == 0);
    sensor->Tick();
    CHECK(sensor->GetSampleCount() == 0);
    CHECK(sensor->GetSamples(0) == nullptr);
}
