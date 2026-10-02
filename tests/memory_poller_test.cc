#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <cstdio>
#include <fstream>
#include <string>

#include "sensors/memory_poller.h"

TEST_CASE("MemoryPoller reads fake meminfo and computes usedRam")
{
    const std::string path = ".obj/test_meminfo_dummy";
    {
        std::ofstream out(path);
        REQUIRE(out.is_open());
        out << "MemTotal:       16384 kB\n";
        out << "MemAvailable:    8192 kB\n";
        out << "Buffers:         1024 kB\n";
        out << "Cached:          2048 kB\n";
    }

    auto poller = CreateMemoryPoller(path);
    REQUIRE(poller != nullptr);
    auto result = poller->Poll();
    REQUIRE(result.size() == 1);
    auto it = result.find("mem");
    REQUIRE(it != result.end());
    CHECK(it->second.totalRam == 16384 * 1024);
    CHECK(it->second.usedRam == (16384 - 8192) * 1024);

    std::remove(path.c_str());
}

TEST_CASE("MemoryPoller handles reordered and spaced meminfo")
{
    const std::string path = ".obj/test_meminfo_reordered";
    {
        std::ofstream out(path);
        REQUIRE(out.is_open());
        out << "MemAvailable:     1024 kB\n";
        out << "Buffers:          512 kB\n";
        out << "MemTotal:       16384 kB\n";
    }

    auto poller = CreateMemoryPoller(path);
    auto result = poller->Poll();
    REQUIRE(result.size() == 1);
    CHECK(result["mem"].totalRam == 16384 * 1024);
    CHECK(result["mem"].usedRam == (16384 - 1024) * 1024);

    std::remove(path.c_str());
}

TEST_CASE("MemoryPoller reflects updated file on next Poll")
{
    const std::string path = ".obj/test_meminfo_update";
    {
        std::ofstream out(path);
        out << "MemTotal:       8000 kB\n";
        out << "MemAvailable:   5500 kB\n";
    }

    auto poller = CreateMemoryPoller(path);
    auto r1 = poller->Poll();
    REQUIRE(r1.size() == 1);
    CHECK(r1["mem"].usedRam == (8000 - 5500) * 1024);

    {
        std::ofstream out(path);
        out << "MemTotal:       8000 kB\n";
        out << "MemAvailable:   6500 kB\n";
    }

    auto r2 = poller->Poll();
    REQUIRE(r2.size() == 1);
    CHECK(r2["mem"].usedRam == (8000 - 6500) * 1024);

    std::remove(path.c_str());
}

TEST_CASE("MemoryPoller handles missing file gracefully")
{
    const std::string missing = ".obj/nonexistent_meminfo";
    std::remove(missing.c_str());
    auto poller = CreateMemoryPoller(missing);
    REQUIRE(poller != nullptr);
    auto result = poller->Poll();
    CHECK(result.empty());
}

TEST_CASE("MemoryPoller returns empty when required fields missing")
{
    const std::string path = ".obj/test_meminfo_incomplete";
    {
        std::ofstream out(path);
        out << "MemTotal:       16384 kB\n";
        out << "MemFree:         8192 kB\n";
    }

    auto poller = CreateMemoryPoller(path);
    auto result = poller->Poll();
    CHECK(result.empty());

    {
        std::ofstream out(path);
        out << "MemAvailable:    8192 kB\n";
        out << "Buffers:         1024 kB\n";
    }
    auto result2 = poller->Poll();
    CHECK(result2.empty());

    std::remove(path.c_str());
}

TEST_CASE("MemoryPoller reads real /proc/meminfo sanity check")
{
    auto poller = CreateMemoryPoller();
    auto result = poller->Poll();
    REQUIRE(result.size() == 1);
    auto it = result.find("mem");
    REQUIRE(it != result.end());
    CHECK(it->second.totalRam > 0);
    CHECK(it->second.usedRam >= 0);
    CHECK(it->second.usedRam <= it->second.totalRam);
}
