#pragma once

#include "memory_poller.h"
#include "sensor.h"
#include "sensor_graph_mixin.h"

#include <memory>
#include <string>

class Preferences;
class Timer;

class MemorySensor : public Sensor, public SensorGraphMixin<MemorySample>
{
public:
    explicit MemorySensor(const std::string& prefPrefix): Sensor(prefPrefix) {}

    virtual ~MemorySensor() = default;
};

extern std::unique_ptr<MemorySensor> CreateMemorySensor(const Preferences& prefs,
    Timer& timer,
    const std::string& prefPrefix,
    const std::string& procMemInfoPath = "/proc/meminfo");

extern std::unique_ptr<MemorySensor> CreateMemorySensor(const Preferences& prefs,
    Timer& timer,
    const std::string& prefPrefix,
    std::shared_ptr<MemoryPoller> poller);
