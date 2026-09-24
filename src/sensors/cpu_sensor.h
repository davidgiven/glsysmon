#pragma once

#include "sensor.h"
#include "sensor_graph_mixin.h"

#include <memory>
#include <string>

class Preferences;
class Timer;

struct CpuSample
{
    float user;
    float system;
    float nice;
};

class CpuSensor : public Sensor, public SensorGraphMixin<CpuSample>
{
public:
    explicit CpuSensor(const std::string& prefPrefix): Sensor(prefPrefix)
    {
    }

    virtual ~CpuSensor() = default;
};

extern std::unique_ptr<CpuSensor> CreateCpuSensor(const Preferences& prefs,
    Timer& timer,
    const std::string& prefPrefix,
    const std::string& procStatPath = "/proc/stat");
