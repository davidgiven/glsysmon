#pragma once

#include "memory_poller.h"
#include "sensor.h"
#include "sensor_graph_mixin.h"

#include <memory>
#include <string>

class Context;

class MemorySensor : public Sensor, public SensorGraphMixin<MemorySample>
{
public:
    explicit MemorySensor(const Context& ctx, const std::string& prefPrefix):
        Sensor(ctx, prefPrefix)
    {
    }

    explicit MemorySensor(const std::string& prefPrefix): Sensor(prefPrefix) {}

    virtual ~MemorySensor() = default;
};

extern std::unique_ptr<MemorySensor> CreateMemorySensor(const Context& ctx,
    const std::string& prefPrefix,
    std::shared_ptr<MemoryPoller> poller);
