#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <functional>
#include <vector>

#include "timer.h"

TEST_CASE("Timer fires only after Tick reaches scheduled time")
{
    auto timer = CreateTimer();
    bool called = false;
    timer->Schedule(10,
        [&](Timer::Time)
        {
            called = true;
        });
    timer->Tick(5);
    CHECK_FALSE(called);
    timer->Tick(10);
    CHECK(called);
}

TEST_CASE("Timer does not fire twice")
{
    auto timer = CreateTimer();
    int count = 0;
    timer->Schedule(10,
        [&](Timer::Time)
        {
            ++count;
        });
    timer->Tick(10);
    CHECK(count == 1);
    timer->Tick(20);
    CHECK(count == 1);
}

TEST_CASE("Timer fires in monotonically increasing order")
{
    auto timer = CreateTimer();
    std::vector<Timer::Time> got;
    timer->Schedule(30,
        [&](Timer::Time s)
        {
            got.push_back(s);
        });
    timer->Schedule(10,
        [&](Timer::Time s)
        {
            got.push_back(s);
        });
    timer->Schedule(20,
        [&](Timer::Time s)
        {
            got.push_back(s);
        });
    timer->Tick(30);
    REQUIRE(got.size() == 3);
    CHECK(got[0] == 10);
    CHECK(got[1] == 20);
    CHECK(got[2] == 30);
}

TEST_CASE("Schedule returns adjusted timestamp for duplicate times")
{
    auto timer = CreateTimer();
    Timer::Time a = timer->Schedule(10,
        [](Timer::Time)
        {
        });
    Timer::Time b = timer->Schedule(10,
        [](Timer::Time)
        {
        });
    Timer::Time c = timer->Schedule(10,
        [](Timer::Time)
        {
        });
    CHECK(a == 10);
    CHECK(b == 11);
    CHECK(c == 12);
}

TEST_CASE("Tick respects adjusted times and FIFO")
{
    auto timer = CreateTimer();
    std::vector<Timer::Time> got;
    timer->Schedule(10,
        [&](Timer::Time s)
        {
            got.push_back(s);
        });
    timer->Schedule(10,
        [&](Timer::Time s)
        {
            got.push_back(s);
        });
    timer->Schedule(10,
        [&](Timer::Time s)
        {
            got.push_back(s);
        });
    timer->Tick(10);
    REQUIRE(got.size() == 1);
    CHECK(got[0] == 10);
    timer->Tick(11);
    REQUIRE(got.size() == 2);
    CHECK(got[1] == 11);
    timer->Tick(12);
    REQUIRE(got.size() == 3);
    CHECK(got[2] == 12);
}

TEST_CASE("Schedule increments until unique across gaps")
{
    auto timer = CreateTimer();
    Timer::Time a = timer->Schedule(10,
        [](Timer::Time)
        {
        });
    Timer::Time b = timer->Schedule(11,
        [](Timer::Time)
        {
        });
    Timer::Time c = timer->Schedule(10,
        [](Timer::Time)
        {
        });
    CHECK(a == 10);
    CHECK(b == 11);
    CHECK(c == 12);
}

TEST_CASE("Cancel removes pending timer")
{
    auto timer = CreateTimer();
    bool called = false;
    Timer::Time h = timer->Schedule(10,
        [&](Timer::Time)
        {
            called = true;
        });
    timer->Cancel(h);
    timer->Tick(20);
    CHECK_FALSE(called);
}

TEST_CASE("Cancel middle of duplicate-time group")
{
    auto timer = CreateTimer();
    std::vector<int> order;
    Timer::Time h1 = timer->Schedule(10,
        [&](Timer::Time)
        {
            order.push_back(1);
        });
    Timer::Time h2 = timer->Schedule(10,
        [&](Timer::Time)
        {
            order.push_back(2);
        });
    Timer::Time h3 = timer->Schedule(10,
        [&](Timer::Time)
        {
            order.push_back(3);
        });
    CHECK(h1 == 10);
    CHECK(h2 == 11);
    CHECK(h3 == 12);
    timer->Cancel(h2);
    timer->Tick(12);
    REQUIRE(order.size() == 2);
    CHECK(order[0] == 1);
    CHECK(order[1] == 3);
}

TEST_CASE("Cancel non-existent time is no-op")
{
    auto timer = CreateTimer();
    timer->Cancel(9999);
    timer->Cancel(0);
    bool called = false;
    timer->Schedule(10,
        [&](Timer::Time)
        {
            called = true;
        });
    timer->Cancel(999);
    timer->Tick(10);
    CHECK(called);
}

TEST_CASE("Callback receives scheduled time")
{
    auto timer = CreateTimer();
    Timer::Time received = 0;
    timer->Schedule(100,
        [&](Timer::Time s)
        {
            received = s;
        });
    timer->Tick(1000);
    CHECK(received == 100);
}

TEST_CASE("Callback receives adjusted time for duplicate")
{
    auto timer = CreateTimer();
    Timer::Time received = 0;
    timer->Schedule(10,
        [](Timer::Time)
        {
        });
    Timer::Time h2 = timer->Schedule(10,
        [&](Timer::Time s)
        {
            received = s;
        });
    CHECK(h2 == 11);
    timer->Tick(11);
    CHECK(received == 11);
}

TEST_CASE("Callback can schedule another timer")
{
    auto timer = CreateTimer();
    int n = 0;
    std::function<void(Timer::Time)> cb;
    cb = [&](Timer::Time s)
    {
        ++n;
        if (n < 3)
            timer->Schedule(s + 10, cb);
    };
    timer->Schedule(10, cb);
    timer->Tick(10);
    CHECK(n == 1);
    timer->Tick(20);
    CHECK(n == 2);
    timer->Tick(30);
    CHECK(n == 3);
    timer->Tick(40);
    CHECK(n == 3);
}

TEST_CASE("Tick catches up and fires chained schedule in same Tick")
{
    auto timer = CreateTimer();
    std::vector<Timer::Time> got;
    timer->Schedule(10,
        [&](Timer::Time s)
        {
            got.push_back(s);
            timer->Schedule(15,
                [&](Timer::Time s2)
                {
                    got.push_back(s2);
                });
        });
    timer->Tick(100);
    REQUIRE(got.size() == 2);
    CHECK(got[0] == 10);
    CHECK(got[1] == 15);
}

TEST_CASE("Tick fires all due timers at once")
{
    auto timer = CreateTimer();
    std::vector<Timer::Time> got;
    timer->Schedule(10,
        [&](Timer::Time s)
        {
            got.push_back(s);
        });
    timer->Schedule(20,
        [&](Timer::Time s)
        {
            got.push_back(s);
        });
    timer->Schedule(30,
        [&](Timer::Time s)
        {
            got.push_back(s);
        });
    timer->Tick(100);
    REQUIRE(got.size() == 3);
    CHECK(got[0] == 10);
    CHECK(got[1] == 20);
    CHECK(got[2] == 30);
}

TEST_CASE("Cancel during Tick prevents later timer")
{
    auto timer = CreateTimer();
    bool b1 = false;
    bool b2 = false;
    Timer::Time h2 = timer->Schedule(20,
        [&](Timer::Time)
        {
            b2 = true;
        });
    timer->Schedule(10,
        [&](Timer::Time)
        {
            b1 = true;
            timer->Cancel(h2);
        });
    timer->Tick(30);
    CHECK(b1);
    CHECK_FALSE(b2);
}

TEST_CASE("Schedule after Cancel reuses time")
{
    auto timer = CreateTimer();
    Timer::Time h1 = timer->Schedule(10,
        [](Timer::Time)
        {
        });
    timer->Cancel(h1);
    Timer::Time h2 = timer->Schedule(10,
        [](Timer::Time)
        {
        });
    CHECK(h2 == 10);
}

TEST_CASE("Schedule returns Time and preserves uint64 nanoseconds")
{
    auto timer = CreateTimer();
    Timer::Time large = 1'000'000'000ULL * 5;
    Timer::Time h = timer->Schedule(large,
        [](Timer::Time)
        {
        });
    CHECK(h == large);
    bool called = false;
    timer->Tick(large);
    // Use a fresh timer to verify Tick boundary
    auto timer2 = CreateTimer();
    timer2->Schedule(large,
        [&](Timer::Time)
        {
            called = true;
        });
    timer2->Tick(large - 1);
    CHECK_FALSE(called);
    timer2->Tick(large);
    CHECK(called);
}
