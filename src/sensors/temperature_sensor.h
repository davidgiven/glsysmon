#pragma once

#include "sensor.h"
#include "sensor_graph_mixin.h"

#include <memory>
#include <string>

class Preferences;
class Timer;

class TemperatureSensor : public Sensor, public SensorGraphMixin<double>
{
public:
    explicit TemperatureSensor(const std::string& prefPrefix): Sensor(prefPrefix)
    {
    }

    virtual ~TemperatureSensor() = default;
};

extern std::unique_ptr<TemperatureSensor> CreateTemperatureSensor(
    const Preferences& prefs,
    Timer& timer,
    const std::string& prefPrefix,
    const std::string& hwmonRoot = "/sys/class/hwmon");
