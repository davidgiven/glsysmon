#pragma once

#include <cstddef>
#include <memory>
#include <span>
#include <string>

#include "sensor.h"

// Sample for a single tick. Each field is in [0, 1] representing the
// proportion of time spent in that usage type during the tick.
struct CpuSample
{
    float user;
    float system;
    float nice;
};

// Fetches CPU usage history for each CPU. Implementations live in
// src/sensors/cpu_sensor_impl.cc.
class CpuSensor : public Sensor
{
public:
    virtual ~CpuSensor() = default;

    // Returns the number of CPUs known to the sensor.
    virtual std::size_t GetCpuCount() = 0;

    // Number of history samples stored per CPU.
    virtual std::size_t GetSampleCount() = 0;

    // Returns a contiguous array of GetSampleCount() CpuSamples for the
    // given CPU. The data is laid out as an array of structs and can be
    // plotted directly with ImPlot/ImGui using stride:
    //
    //   const CpuSample* s = sensor.GetSamples(cpu);
    //   int n = (int)sensor.GetSampleCount();
    //   ImPlot::PlotLine("user", &s[0].user, n, 1, 0, 0, 0,
    //                    sizeof(CpuSample));
    //   ImPlot::PlotLine("system", &s[0].system, n, 1, 0, 0, 0,
    //                    sizeof(CpuSample));
    //   ImPlot::PlotLine("nice", &s[0].nice, n, 1, 0, 0, 0,
    //                    sizeof(CpuSample));
    //   ImGui::PlotLines("user", &s[0].user, n, 0, nullptr, FLT_MAX,
    //                    FLT_MAX, ImVec2(0, 0), sizeof(CpuSample));
    //
    // Each CpuSample field is in [0, 1]. The returned pointer remains
    // valid until the next sample update and is owned by the sensor.
    virtual const CpuSample* GetSamples(std::size_t cpu) = 0;

    // Advances the history by one tick: opens /proc/stat, reads each
    // numbered cpu line and appends a CpuSample for each CPU.
    virtual void Tick() = 0;

    // Const overloads for callers that hold a const sensor. They forward
    // to the non-const versions via const_cast, so a sensor only needs to
    // implement the non-const versions.
    std::size_t GetCpuCount() const
    {
        return const_cast<CpuSensor*>(this)->GetCpuCount();
    }

    std::size_t GetSampleCount() const
    {
        return const_cast<CpuSensor*>(this)->GetSampleCount();
    }

    const CpuSample* GetSamples(std::size_t cpu) const
    {
        return const_cast<CpuSensor*>(this)->GetSamples(cpu);
    }

    // Span-based accessor — also suitable for plotting via
    // PlotLine(..., span.data()->user, ..., sizeof(CpuSample)).
    std::span<const CpuSample> GetSamplesSpan(std::size_t cpu)
    {
        const CpuSample* data = GetSamples(cpu);
        if (data == nullptr)
            return {};
        return {data, GetSampleCount()};
    }

    std::span<const CpuSample> GetSamplesSpan(std::size_t cpu) const
    {
        const CpuSample* data = GetSamples(cpu);
        if (data == nullptr)
            return {};
        return {data, GetSampleCount()};
    }
};

extern std::unique_ptr<CpuSensor> CreateCpuSensor(
    const std::string& procStatPath = "/proc/stat");
