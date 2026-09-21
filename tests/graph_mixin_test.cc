#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <cstdio>
#include <fstream>
#include <memory>
#include <span>
#include <string>
#include <vector>

#include "components.h"
#include "sensors/cpu_sensor.h"
#include "sensors/graph_mixin.h"

namespace
{

    template <typename T>
    class TestGraph : public GraphMixin<T>
    {
    public:
        using GraphMixin<T>::AddSample;
        using GraphMixin<T>::GraphMixin;
        using GraphMixin<T>::InitGraph;

        std::string GetChannelName(std::size_t channel) const override
        {
            return "Channel" + std::to_string(channel);
        }
    };

    struct Sample
    {
        float user = 0.0f;
        float system = 0.0f;
        bool operator==(const Sample& other) const
        {
            return user == other.user && system == other.system;
        }
    };

} // namespace

TEST_CASE("GraphMixin default is empty")
{
    TestGraph<int> g;
    CHECK(g.GetChannels() == 0);
    CHECK(g.GetSampleCount() == 0);
    const auto& cg = static_cast<const TestGraph<int>&>(g);
    CHECK(cg.GetChannels() == 0);
    CHECK(cg.GetSampleCount() == 0);
}

TEST_CASE("GraphMixin constructor initializes channels and samples")
{
    TestGraph<int> g(2, 3, 42);
    CHECK(g.GetChannels() == 2);
    CHECK(g.GetSampleCount() == 3);
    for (std::size_t c = 0; c < 2; ++c)
    {
        const int* s = g.GetSamples(c);
        REQUIRE(s != nullptr);
        for (std::size_t i = 0; i < 3; ++i)
            CHECK(s[i] == 42);
    }
    // const overload
    const TestGraph<int>& cg = g;
    CHECK(cg.GetChannels() == 2);
    CHECK(cg.GetSampleCount() == 3);
    REQUIRE(cg.GetSamples(0) != nullptr);
    CHECK(cg.GetSamples(0)[0] == 42);
}

TEST_CASE("GraphMixin InitGraph creates and resets graph")
{
    TestGraph<int> g;
    g.InitGraph(3, 4, 7);
    CHECK(g.GetChannels() == 3);
    CHECK(g.GetSampleCount() == 4);
    for (std::size_t c = 0; c < 3; ++c)
    {
        const int* s = g.GetSamples(c);
        REQUIRE(s != nullptr);
        for (std::size_t i = 0; i < 4; ++i)
            CHECK(s[i] == 7);
    }
    // re-init with different size clears old data
    g.InitGraph(1, 2, 9);
    CHECK(g.GetChannels() == 1);
    CHECK(g.GetSampleCount() == 2);
    const int* s = g.GetSamples(0);
    REQUIRE(s != nullptr);
    CHECK(s[0] == 9);
    CHECK(s[1] == 9);
}

TEST_CASE("GraphMixin AddSample shifts history per channel")
{
    TestGraph<int> g(1, 3, 0);
    // initial [0,0,0]
    const int* s0 = g.GetSamples(0);
    REQUIRE(s0 != nullptr);
    CHECK(s0[0] == 0);
    CHECK(s0[1] == 0);
    CHECK(s0[2] == 0);

    g.AddSample(0, 1);
    // [0,0,1]
    s0 = g.GetSamples(0);
    CHECK(s0[0] == 0);
    CHECK(s0[1] == 0);
    CHECK(s0[2] == 1);

    g.AddSample(0, 2);
    // [0,1,2]
    s0 = g.GetSamples(0);
    CHECK(s0[0] == 0);
    CHECK(s0[1] == 1);
    CHECK(s0[2] == 2);

    g.AddSample(0, 3);
    // [1,2,3]
    s0 = g.GetSamples(0);
    CHECK(s0[0] == 1);
    CHECK(s0[1] == 2);
    CHECK(s0[2] == 3);

    g.AddSample(0, 4);
    // [2,3,4]
    s0 = g.GetSamples(0);
    CHECK(s0[0] == 2);
    CHECK(s0[1] == 3);
    CHECK(s0[2] == 4);
}

TEST_CASE("GraphMixin channels are independent")
{
    TestGraph<int> g(2, 3, 0);
    g.AddSample(0, 1);
    g.AddSample(0, 2);
    g.AddSample(1, 10);
    g.AddSample(1, 20);

    const int* s0 = g.GetSamples(0);
    const int* s1 = g.GetSamples(1);
    REQUIRE(s0 != nullptr);
    REQUIRE(s1 != nullptr);
    // s0: [0,1,2]
    CHECK(s0[0] == 0);
    CHECK(s0[1] == 1);
    CHECK(s0[2] == 2);
    // s1: [0,10,20]
    CHECK(s1[0] == 0);
    CHECK(s1[1] == 10);
    CHECK(s1[2] == 20);

    g.AddSample(0, 3);
    // s0 shifts, s1 unchanged
    CHECK(s0[0] == 1);
    CHECK(s0[1] == 2);
    CHECK(s0[2] == 3);
    CHECK(s1[0] == 0);
    CHECK(s1[1] == 10);
    CHECK(s1[2] == 20);
}

TEST_CASE("GraphMixin AddSample with sampleCount 1 overwrites")
{
    TestGraph<int> g(2, 1, 0);
    CHECK(g.GetSampleCount() == 1);
    g.AddSample(0, 5);
    CHECK(g.GetSamples(0)[0] == 5);
    g.AddSample(0, 6);
    CHECK(g.GetSamples(0)[0] == 6);
    g.AddSample(1, 7);
    CHECK(g.GetSamples(1)[0] == 7);
    // channel 0 unchanged by channel 1
    CHECK(g.GetSamples(0)[0] == 6);
}

TEST_CASE("GraphMixin GetSamplesSpan returns span")
{
    TestGraph<int> g(1, 3, 0);
    g.AddSample(0, 1);
    g.AddSample(0, 2);
    g.AddSample(0, 3);
    // span should be [1,2,3]
    auto span = g.GetSamplesSpan(0);
    REQUIRE(span.size() == 3);
    CHECK(span[0] == 1);
    CHECK(span[1] == 2);
    CHECK(span[2] == 3);

    const TestGraph<int>& cg = g;
    auto cspan = cg.GetSamplesSpan(0);
    REQUIRE(cspan.size() == 3);
    CHECK(cspan[0] == 1);
}

TEST_CASE("GraphMixin works with struct sample type")
{
    TestGraph<Sample> g(1, 2, Sample{0.0f, 0.0f});
    g.AddSample(0, Sample{0.2f, 0.1f});
    g.AddSample(0, Sample{0.5f, 0.3f});
    const Sample* s = g.GetSamples(0);
    REQUIRE(s != nullptr);
    CHECK(s[0] == Sample{0.2f, 0.1f});
    CHECK(s[1] == Sample{0.5f, 0.3f});
}

TEST_CASE("GraphMixin virtual dispatch via base pointer")
{
    TestGraph<int> impl(2, 2, 1);
    GraphMixin<int>* base = &impl;
    CHECK(base->GetChannels() == 2);
    CHECK(base->GetSampleCount() == 2);
    REQUIRE(base->GetSamples(0) != nullptr);
    CHECK(base->GetSamples(0)[0] == 1);

    const GraphMixin<int>* cbase = &impl;
    CHECK(cbase->GetChannels() == 2);
    CHECK(cbase->GetSampleCount() == 2);
}

TEST_CASE("GraphMixin GetChannelName returns channel name")
{
    TestGraph<int> g(2, 2, 0);
    CHECK(g.GetChannelName(0) == "Channel0");
    CHECK(g.GetChannelName(1) == "Channel1");
    const GraphMixin<int>& cg = g;
    CHECK(cg.GetChannelName(0) == "Channel0");
    CHECK(cg.GetChannelName(1) == "Channel1");

    GraphMixin<int>* base = &g;
    CHECK(base->GetChannelName(0) == "Channel0");
    CHECK(base->GetChannelName(2) == "Channel2");
}

TEST_CASE("CpuSensorImpl GetChannelName returns CPU number")
{
    CliArgs args;
    auto prefs = CreatePreferences(args);
    const std::string path = ".obj/test_graph_mixin_cpu_stat";
    {
        std::ofstream out(path);
        out << "cpu  100 0 100 1000 0 0 0 0 0 0\n";
        out << "cpu0 50 0 50 500 0 0 0 0 0 0\n";
        out << "cpu1 50 0 50 500 0 0 0 0 0 0\n";
        out << "cpu2 50 0 50 500 0 0 0 0 0 0\n";
    }
    auto timer = CreateTimer();
    auto sensor = CreateCpuSensor(*prefs, *timer, path);
    auto* gm = dynamic_cast<GraphMixin<CpuSample>*>(sensor.get());
    REQUIRE(gm != nullptr);
    CHECK(gm->GetChannelName(0) == "CPU0");
    CHECK(gm->GetChannelName(1) == "CPU1");
    CHECK(gm->GetChannelName(2) == "CPU2");
    // also via const
    const auto* cgm = dynamic_cast<const GraphMixin<CpuSample>*>(sensor.get());
    REQUIRE(cgm != nullptr);
    CHECK(cgm->GetChannelName(0) == "CPU0");
    std::remove(path.c_str());
}
