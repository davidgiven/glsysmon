#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <string>
#include <thread>

#include "preferences/preferences.h"
#include "sensors/memory_poller.h"
#include "sensors/poller.h"
#include "sensors/sensors.h"
#include "timer.h"

namespace
{

    struct DummySample
    {
        int value = 0;
        bool operator==(const DummySample& other) const
        {
            return value == other.value;
        }
    };

    class FakePoller : public Poller<DummySample>
    {
    public:
        int pollCount = 0;
        int current = 0;
        std::chrono::steady_clock::time_point fakeNow =
            std::chrono::steady_clock::now();

        std::map<std::string, DummySample> Poll() override
        {
            ++pollCount;
            ++current;
            return {
                {"k", DummySample{current}}
            };
        }

    protected:
        std::chrono::steady_clock::time_point Now() const override
        {
            return fakeNow;
        }
    };

    struct ScopedEnv
    {
        std::string key;
        std::string oldValue;
        bool hadOld = false;

        explicit ScopedEnv(const std::string& k, const std::string& v): key(k)
        {
            const char* old = std::getenv(k.c_str());
            if (old != nullptr)
            {
                hadOld = true;
                oldValue = old;
            }
            setenv(k.c_str(), v.c_str(), 1);
        }

        ~ScopedEnv()
        {
            if (hadOld)
                setenv(key.c_str(), oldValue.c_str(), 1);
            else
                unsetenv(key.c_str());
        }
    };

    std::string MakeTempDir()
    {
        char tmpl[] = "/tmp/glsysmon_poller_test_XXXXXX";
        char* dir = mkdtemp(tmpl);
        REQUIRE(dir != nullptr);
        return std::string(dir);
    }

} // namespace

TEST_CASE("Poller::PollCached default interval is 500ms")
{
    FakePoller poller;
    CHECK(poller.GetCacheIntervalMs() == 500);
    CHECK(poller.GetCacheInterval() == std::chrono::milliseconds(500));

    auto map = CreateMapPreferences();
    CHECK_FALSE(map->GetInteger("poller.cache_interval").has_value());
    CHECK(GlobalPreferencesFetcher::GetPollerCacheInterval(*map) == 500);
}

TEST_CASE("Poller::PollCached caches within interval")
{
    FakePoller poller;

    auto r1 = poller.PollCached();
    CHECK(poller.pollCount == 1);
    CHECK(r1.at("k").value == 1);

    auto r2 = poller.PollCached();
    CHECK(poller.pollCount == 1);
    CHECK(r2.at("k").value == 1);

    poller.fakeNow += std::chrono::milliseconds(400);
    auto r3 = poller.PollCached();
    CHECK(poller.pollCount == 1);
    CHECK(r3.at("k").value == 1);
}

TEST_CASE("Poller::PollCached repolls after interval expires")
{
    FakePoller poller;

    auto r1 = poller.PollCached();
    CHECK(poller.pollCount == 1);

    poller.fakeNow += std::chrono::milliseconds(600);
    auto r2 = poller.PollCached();
    CHECK(poller.pollCount == 2);
    CHECK(r2.at("k").value == 2);

    // Immediately after should still be cached
    auto r3 = poller.PollCached();
    CHECK(poller.pollCount == 2);
    CHECK(r3.at("k").value == 2);

    poller.fakeNow += std::chrono::milliseconds(500);
    auto r4 = poller.PollCached();
    CHECK(poller.pollCount == 3);
    CHECK(r4.at("k").value == 3);
}

TEST_CASE("Poller::PollCached respects custom interval via SetCacheIntervalMs")
{
    FakePoller poller;
    poller.SetCacheIntervalMs(100);
    CHECK(poller.GetCacheIntervalMs() == 100);

    auto r1 = poller.PollCached();
    CHECK(poller.pollCount == 1);

    poller.fakeNow += std::chrono::milliseconds(50);
    auto r2 = poller.PollCached();
    CHECK(poller.pollCount == 1);

    poller.fakeNow += std::chrono::milliseconds(60);
    auto r3 = poller.PollCached();
    CHECK(poller.pollCount == 2);
    CHECK(r3.at("k").value == 2);

    poller.SetCacheInterval(std::chrono::milliseconds(200));
    CHECK(poller.GetCacheIntervalMs() == 200);

    poller.fakeNow += std::chrono::milliseconds(100);
    auto r4 = poller.PollCached();
    CHECK(poller.pollCount == 2);

    poller.fakeNow += std::chrono::milliseconds(120);
    auto r5 = poller.PollCached();
    CHECK(poller.pollCount == 3);
}

TEST_CASE("Poller::PollCached with MemoryPoller caches file reads")
{
    const std::string path = ".obj/test_poller_meminfo_cache";
    {
        std::ofstream out(path);
        REQUIRE(out.is_open());
        out << "MemTotal:       8000 kB\n";
        out << "MemFree:        2000 kB\n";
        out << "Buffers:         500 kB\n";
    }

    auto poller = CreateMemoryPoller(path);
    REQUIRE(poller != nullptr);
    CHECK(poller->GetCacheIntervalMs() == 500);

    auto r1 = poller->PollCached();
    REQUIRE(r1.size() == 1);
    CHECK(r1["mem"].usedRam == (8000 - 2000 - 500) * 1024);

    {
        std::ofstream out(path);
        out << "MemTotal:       8000 kB\n";
        out << "MemFree:        1000 kB\n";
        out << "Buffers:         500 kB\n";
    }

    auto r2 = poller->PollCached();
    CHECK(r2["mem"].usedRam == (8000 - 2000 - 500) * 1024);

    auto rDirect = poller->Poll();
    CHECK(rDirect["mem"].usedRam == (8000 - 1000 - 500) * 1024);

    std::this_thread::sleep_for(std::chrono::milliseconds(600));
    auto r3 = poller->PollCached();
    CHECK(r3["mem"].usedRam == (8000 - 1000 - 500) * 1024);

    std::remove(path.c_str());
}

TEST_CASE("Poller::PollCached with MemoryPoller respects custom interval")
{
    const std::string path = ".obj/test_poller_meminfo_custom";
    {
        std::ofstream out(path);
        out << "MemTotal:       8000 kB\n";
        out << "MemFree:        2000 kB\n";
        out << "Buffers:         500 kB\n";
    }

    auto poller = CreateMemoryPoller(path);
    REQUIRE(poller != nullptr);
    poller->SetCacheIntervalMs(100);

    auto r1 = poller->PollCached();
    CHECK(r1["mem"].usedRam == (8000 - 2000 - 500) * 1024);

    {
        std::ofstream out(path);
        out << "MemTotal:       8000 kB\n";
        out << "MemFree:        500 kB\n";
        out << "Buffers:         500 kB\n";
    }

    auto r2 = poller->PollCached();
    CHECK(r2["mem"].usedRam == (8000 - 2000 - 500) * 1024);

    std::this_thread::sleep_for(std::chrono::milliseconds(150));
    auto r3 = poller->PollCached();
    CHECK(r3["mem"].usedRam == (8000 - 500 - 500) * 1024);

    std::remove(path.c_str());
}

TEST_CASE(
    "GlobalPreferencesFetcher poller.cache_interval defaults and overrides")
{
    auto map = CreateMapPreferences();
    CHECK(GlobalPreferencesFetcher::GetPollerCacheInterval(*map) == 500);

    map->SetInteger("poller.cache_interval", 200);
    CHECK(GlobalPreferencesFetcher::GetPollerCacheInterval(*map) == 200);

    auto mapMs = CreateMapPreferences();
    mapMs->SetInteger("poller.cache_interval_ms", 123);
    CHECK(GlobalPreferencesFetcher::GetPollerCacheInterval(*mapMs) == 123);

    auto mapUnderscore = CreateMapPreferences();
    mapUnderscore->SetInteger("poller_cache_interval", 456);
    CHECK(GlobalPreferencesFetcher::GetPollerCacheInterval(*mapUnderscore) ==
          456);

    CliArgs args;
    args.values = {"--poller.cache_interval=777"};
    auto cli = CreateCliPreferences(args);
    CHECK(cli->GetInteger("poller.cache_interval").value() == 777);
    CHECK(GlobalPreferencesFetcher::GetPollerCacheInterval(*cli) == 777);
}

TEST_CASE("TomlPreferences reads poller.cache_interval")
{
    const std::string tmp = MakeTempDir();
    ScopedEnv env("XDG_CONFIG_HOME", tmp);

    {
        std::filesystem::path dir = std::filesystem::path(tmp) / "glsysmon";
        std::filesystem::create_directories(dir);
        std::ofstream out(dir / "config.toml");
        REQUIRE(out.is_open());
        out << "poller.cache_interval = 321\n";
    }

    auto prefs = CreateTomlPreferences();
    REQUIRE(prefs->GetInteger("poller.cache_interval").has_value());
    CHECK(prefs->GetInteger("poller.cache_interval").value() == 321);

    CliArgs args;
    auto combined = CreatePreferences(args);
    CHECK(GlobalPreferencesFetcher::GetPollerCacheInterval(*combined) == 321);

    std::filesystem::remove_all(tmp);
}

TEST_CASE("CombinedPreferences prefers CLI over TOML for poller.cache_interval")
{
    const std::string tmp = MakeTempDir();
    ScopedEnv env("XDG_CONFIG_HOME", tmp);

    {
        std::filesystem::path dir = std::filesystem::path(tmp) / "glsysmon";
        std::filesystem::create_directories(dir);
        std::ofstream out(dir / "config.toml");
        REQUIRE(out.is_open());
        out << "poller.cache_interval = 111\n";
    }

    CliArgs args;
    args.values = {"--poller.cache_interval=222"};
    auto prefs = CreatePreferences(args);
    CHECK(prefs->GetInteger("poller.cache_interval").value() == 222);
    CHECK(GlobalPreferencesFetcher::GetPollerCacheInterval(*prefs) == 222);

    std::filesystem::remove_all(tmp);
}

TEST_CASE("Sensors factory propagates poller cache interval from preferences")
{
    auto mapPrefs = CreateMapPreferences();
    mapPrefs->SetInteger("poller.cache_interval", 200);
    auto timer = CreateTimer();
    Sensors sensors(*mapPrefs, *timer);

    const std::string path = ".obj/test_poller_sensors_interval";
    {
        std::ofstream out(path);
        out << "MemTotal:       8000 kB\n";
        out << "MemFree:        2000 kB\n";
        out << "Buffers:         500 kB\n";
    }

    auto memPoller = sensors.CreateMemoryPoller(path);
    REQUIRE(memPoller != nullptr);
    CHECK(memPoller->GetCacheIntervalMs() == 200);

    auto netPoller = sensors.CreateNetworkPoller(path);
    REQUIRE(netPoller != nullptr);
    CHECK(netPoller->GetCacheIntervalMs() == 200);

    mapPrefs->SetInteger("poller.cache_interval", 300);
    auto memPoller2 = sensors.CreateMemoryPoller(path);
    CHECK(memPoller == memPoller2);
    CHECK(memPoller2->GetCacheIntervalMs() == 300);

    auto netPoller2 = sensors.CreateNetworkPoller(path);
    CHECK(netPoller == netPoller2);
    CHECK(netPoller2->GetCacheIntervalMs() == 300);

    std::remove(path.c_str());
}
