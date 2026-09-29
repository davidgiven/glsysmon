#pragma once

#include "cpu_poller.h"
#include "sensor.h"
#include "sensor_graph_mixin.h"

#include <memory>
#include <string>

class Context;

class CpuSensor : public Sensor, public SensorGraphMixin<CpuSample>
{
public:
    explicit CpuSensor(const Context& ctx, const std::string& prefPrefix):
        Sensor(ctx, prefPrefix)
    {
    }

    explicit CpuSensor(const std::string& prefPrefix): Sensor(prefPrefix) {}

    virtual ~CpuSensor() = default;
};

extern std::unique_ptr<CpuSensor> CreateCpuSensor(const Context& ctx,
    const std::string& prefPrefix,
    std::shared_ptr<CpuPoller> poller);
