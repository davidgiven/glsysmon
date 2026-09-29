#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <cstdio>
#include <fstream>
#include <string>
#include <thread>

#include "app.h"
#include "context.h"
#include "imguiif.h"
#include "preferences/preferences.h"
#include "sensors/cpu_poller.h"
#include "sensors/sensors.h"
#include "timer.h"

TEST_CASE("CpuPoller first Poll returns zeros")
{
    const std::string path = ".obj/test_cpu_first";
    {
        std::ofstream out(path);
        REQUIRE(out.is_open());
        out << "cpu  100 0 100 1000 0 0 0 0 0 0\n";
        out << "cpu0 50 0 50 500 0 0 0 0 0 0\n";
        out << "cpu1 50 0 50 500 0 0 0 0 0 0\n";
    }

    auto poller = CreateCpuPoller(path);
    REQUIRE(poller != nullptr);
    auto r1 = poller->Poll();
    REQUIRE(r1.size() == 2);
    REQUIRE(r1.find("0") != r1.end());
    REQUIRE(r1.find("1") != r1.end());
    CHECK(r1["0"].user == doctest::Approx(0.0f));
    CHECK(r1["0"].system == doctest::Approx(0.0f));
    CHECK(r1["0"].nice == doctest::Approx(0.0f));
    CHECK(r1["1"].user == doctest::Approx(0.0f));
    CHECK(r1["1"].system == doctest::Approx(0.0f));
    CHECK(r1["1"].nice == doctest::Approx(0.0f));

    std::remove(path.c_str());
}

TEST_CASE("CpuPoller computes fractions on second Poll")
{
    const std::string path = ".obj/test_cpu_fractions";
    {
        std::ofstream out(path);
        REQUIRE(out.is_open());
        out << "cpu  100 0 100 1000 0 0 0 0 0 0\n";
        out << "cpu0 50 0 50 500 0 0 0 0 0 0\n";
        out << "cpu1 50 0 50 500 0 0 0 0 0 0\n";
    }

    auto poller = CreateCpuPoller(path);
    auto r1 = poller->Poll();
    REQUIRE(r1.size() == 2);

    // second Poll with identical file -> totalDelta 0 -> zeros
    auto r2 = poller->Poll();
    REQUIRE(r2.size() == 2);
    CHECK(r2["0"].user == doctest::Approx(0.0f));
    CHECK(r2["0"].system == doctest::Approx(0.0f));
    CHECK(r2["0"].nice == doctest::Approx(0.0f));

    {
        std::ofstream out(path);
        REQUIRE(out.is_open());
        out << "cpu  200 0 200 1200 0 0 0 0 0 0\n";
        out << "cpu0 100 0 100 600 0 0 0 0 0 0\n";
        out << "cpu1 100 0 100 600 0 0 0 0 0 0\n";
    }

    auto r3 = poller->Poll();
    REQUIRE(r3.size() == 2);
    // prevIdle 500, nowIdle 600 => +100
    // prevNonIdle user+system =100, now 200 => +100
    // totalDelta 200 => user 50/200 =0.25
    CHECK(r3["0"].user == doctest::Approx(0.25f));
    CHECK(r3["0"].system == doctest::Approx(0.25f));
    CHECK(r3["0"].nice == doctest::Approx(0.0f));
    CHECK(r3["1"].user == doctest::Approx(0.25f));
    CHECK(r3["1"].system == doctest::Approx(0.25f));

    std::remove(path.c_str());
}

TEST_CASE("CpuPoller reflects updated file on next Poll with nice")
{
    const std::string path = ".obj/test_cpu_nice";
    {
        std::ofstream out(path);
        REQUIRE(out.is_open());
        out << "cpu0 100 10 20 500 0 0 0 0 0 0\n";
        out << "cpu1 100 10 20 500 0 0 0 0 0 0\n";
    }

    auto poller = CreateCpuPoller(path);
    auto r1 = poller->Poll();
    REQUIRE(r1.size() == 2);

    {
        std::ofstream out(path);
        REQUIRE(out.is_open());
        out << "cpu0 150 20 30 600 0 0 0 0 0 0\n";
        out << "cpu1 150 20 30 600 0 0 0 0 0 0\n";
    }

    auto r2 = poller->Poll();
    REQUIRE(r2.size() == 2);
    // prevIdle 500 now 600 =>100
    // prevNonIdle 130 now 200 =>70? Actually user+nice+system =130 vs 200 =>
    // +70 totalDelta 170 => user 50/170, nice 10/170, system 10/170
    CHECK(r2["0"].user == doctest::Approx(50.0f / 170.0f));
    CHECK(r2["0"].nice == doctest::Approx(10.0f / 170.0f));
    CHECK(r2["0"].system == doctest::Approx(10.0f / 170.0f));

    std::remove(path.c_str());
}

TEST_CASE("CpuPoller handles missing file gracefully")
{
    const std::string missing = ".obj/nonexistent_cpu_stat";
    std::remove(missing.c_str());
    auto poller = CreateCpuPoller(missing);
    REQUIRE(poller != nullptr);
    auto result = poller->Poll();
    CHECK(result.empty());

    // second poll still empty
    auto result2 = poller->Poll();
    CHECK(result2.empty());
}

TEST_CASE("CpuPoller ignores aggregate cpu line")
{
    const std::string path = ".obj/test_cpu_aggregate";
    {
        std::ofstream out(path);
        REQUIRE(out.is_open());
        out << "cpu  1000 0 1000 5000 0 0 0 0 0 0\n";
        out << "cpu0 100 0 100 1000 0 0 0 0 0 0\n";
    }

    auto poller = CreateCpuPoller(path);
    auto r1 = poller->Poll();
    REQUIRE(r1.size() == 1);
    CHECK(r1.find("0") != r1.end());
    CHECK(r1.find("cpu") == r1.end());

    std::remove(path.c_str());
}

TEST_CASE("CpuPoller handles newly appeared cpu")
{
    const std::string path = ".obj/test_cpu_new_cpu";
    {
        std::ofstream out(path);
        REQUIRE(out.is_open());
        out << "cpu0 50 0 50 500 0 0 0 0 0 0\n";
        out << "cpu1 50 0 50 500 0 0 0 0 0 0\n";
    }

    auto poller = CreateCpuPoller(path);
    auto r1 = poller->Poll();
    REQUIRE(r1.size() == 2);

    {
        std::ofstream out(path);
        REQUIRE(out.is_open());
        out << "cpu0 100 0 100 600 0 0 0 0 0 0\n";
        out << "cpu1 100 0 100 600 0 0 0 0 0 0\n";
        out << "cpu2 50 0 50 500 0 0 0 0 0 0\n";
    }

    auto r2 = poller->Poll();
    REQUIRE(r2.size() == 3);
    CHECK(r2["0"].user == doctest::Approx(0.25f));
    CHECK(r2["1"].user == doctest::Approx(0.25f));
    // new cpu had no prev -> zero
    CHECK(r2["2"].user == doctest::Approx(0.0f));
    CHECK(r2["2"].system == doctest::Approx(0.0f));
    CHECK(r2["2"].nice == doctest::Approx(0.0f));

    std::remove(path.c_str());
}

TEST_CASE("CpuPoller via Sensors factory")
{
    CliArgs args;
    auto prefs = CreatePreferences(args);
    auto timer = CreateTimer();
    auto imgui = CreateImGui();
    struct DummyApp : public App
    {
        void Setup() override {}
        void MainLoop() override {}
        void Shutdown() override {}
        std::shared_ptr<Preferences> GetPreferences() override
        {
            return nullptr;
        }
        Context& GetContext() override
        {
            static auto dp = CreateMapPreferences();
            static auto dt = CreateTimer();
            static auto di = CreateImGui();
            static Context dc(*this, *di, *dp, *dt);
            return dc;
        }
        void Quit() override {}
    } dummyApp;
    Context ctx(dummyApp, *imgui, *prefs, *timer);
    Sensors sensors(ctx);

    const std::string path = ".obj/test_cpu_sensors";
    {
        std::ofstream out(path);
        REQUIRE(out.is_open());
        out << "cpu0 10 0 10 100 0 0 0 0 0 0\n";
        out << "cpu1 10 0 10 100 0 0 0 0 0 0\n";
    }

    auto poller = sensors.CreateCpuPoller(path);
    REQUIRE(poller != nullptr);
    auto r1 = poller->Poll();
    REQUIRE(r1.size() == 2);
    CHECK(r1["0"].user == doctest::Approx(0.0f));

    {
        std::ofstream out(path);
        REQUIRE(out.is_open());
        out << "cpu0 20 0 20 120 0 0 0 0 0 0\n";
        out << "cpu1 20 0 20 120 0 0 0 0 0 0\n";
    }

    auto r2 = poller->Poll();
    REQUIRE(r2.size() == 2);
    // idle 100->120 +20, nonIdle 20->40 +20 => total 40 => user 10/40=0.25
    CHECK(r2["0"].user == doctest::Approx(0.25f));

    auto poller2 = sensors.CreateCpuPoller(path);
    CHECK(poller == poller2);

    std::remove(path.c_str());
}

TEST_CASE("CpuPoller reads real /proc/stat sanity check")
{
    auto poller = CreateCpuPoller();
    auto r1 = poller->Poll();
    REQUIRE(!r1.empty());
    for (const auto& kv : r1)
    {
        CHECK(kv.second.user == doctest::Approx(0.0f));
        CHECK(kv.second.system == doctest::Approx(0.0f));
        CHECK(kv.second.nice == doctest::Approx(0.0f));
    }

    auto r2 = poller->Poll();
    REQUIRE(r2.size() == r1.size());
    for (const auto& kv : r2)
    {
        CHECK(kv.second.user >= 0.0f);
        CHECK(kv.second.user <= 1.0f);
        CHECK(kv.second.system >= 0.0f);
        CHECK(kv.second.system <= 1.0f);
        CHECK(kv.second.nice >= 0.0f);
        CHECK(kv.second.nice <= 1.0f);
        CHECK(
            kv.second.user + kv.second.system + kv.second.nice <= 1.0f + 1e-5f);
    }
}

TEST_CASE("CpuPoller PollCached caches file reads")
{
    const std::string path = ".obj/test_cpu_poller_cache";
    {
        std::ofstream out(path);
        REQUIRE(out.is_open());
        out << "cpu0 50 0 50 500 0 0 0 0 0 0\n";
        out << "cpu1 50 0 50 500 0 0 0 0 0 0\n";
    }

    auto poller = CreateCpuPoller(path);
    REQUIRE(poller != nullptr);
    CHECK(poller->GetCacheIntervalMs() == 500);

    auto r1 = poller->PollCached();
    REQUIRE(r1.size() == 2);
    CHECK(r1["0"].user == doctest::Approx(0.0f));

    // update file but PollCached should still return cached zeros
    {
        std::ofstream out(path);
        REQUIRE(out.is_open());
        out << "cpu0 100 0 100 600 0 0 0 0 0 0\n";
        out << "cpu1 100 0 100 600 0 0 0 0 0 0\n";
    }

    auto r2 = poller->PollCached();
    CHECK(r2["0"].user == doctest::Approx(0.0f));

    auto rDirect = poller->Poll();
    CHECK(rDirect["0"].user == doctest::Approx(0.25f));

    std::this_thread::sleep_for(std::chrono::milliseconds(600));
    auto r3 = poller->PollCached();
    // after interval, should repoll. rDirect consumed the update, so next delta
    // is 0 rDirect's cur became prev, current file same as rDirect's cur =>
    // totalDelta 0 => 0
    CHECK(r3["0"].user == doctest::Approx(0.0f));

    std::remove(path.c_str());
}
