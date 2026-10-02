#pragma once

#include "app.h"

#include "memory_poller.h"
#include "sensor.h"
#include "sensor_graph_mixin.h"

#include <memory>
#include <string>

class App;

class MemorySensor : public Sensor, public SensorGraphMixin<MemorySample>
{
public:
    explicit MemorySensor(App& app, const std::string& prefPrefix):
        Sensor(app, prefPrefix)
    {
    }

    explicit MemorySensor(const std::string& prefPrefix): Sensor(prefPrefix) {}

    virtual ~MemorySensor() = default;
};

extern std::unique_ptr<MemorySensor> CreateMemorySensor(App& app,
    const std::string& prefPrefix,
    std::shared_ptr<MemoryPoller> poller);
