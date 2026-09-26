#pragma once

#include "sensor.h"
#include "sensor_graph_mixin.h"

#include <memory>
#include <string>

class Preferences;
class Timer;

struct DiskSample
{
    double txBps;
    double rxBps;
};

class DiskSensor : public Sensor, public SensorGraphMixin<DiskSample>
{
public:
    explicit DiskSensor(const std::string& prefPrefix): Sensor(prefPrefix) {}

    virtual ~DiskSensor() = default;
};

extern std::unique_ptr<DiskSensor> CreateDiskSensor(const Preferences& prefs,
    Timer& timer,
    const std::string& prefPrefix,
    const std::string& procDiskStatsPath = "/proc/diskstats");
