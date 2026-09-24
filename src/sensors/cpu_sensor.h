#pragma once

#include "graph_mixin.h"
#include "sensor.h"

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

class CpuSensor : public Sensor, public GraphMixin<CpuSample>
{
public:
    virtual ~CpuSensor() = default;
};

extern std::unique_ptr<CpuSensor> CreateCpuSensor(
    const Preferences& prefs, Timer& timer, const std::string& procStatPath);
extern std::unique_ptr<CpuSensor> CreateCpuSensor(
    const Preferences& prefs, Timer& timer);
