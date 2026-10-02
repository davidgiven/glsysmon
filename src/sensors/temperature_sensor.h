#pragma once

#include "app.h"

#include "sensor.h"
#include "sensor_graph_mixin.h"

#include <memory>
#include <string>

class App;

class TemperatureSensor : public Sensor, public SensorGraphMixin<double>
{
public:
    explicit TemperatureSensor(App& app, const std::string& prefPrefix):
        Sensor(app, prefPrefix)
    {
    }

    explicit TemperatureSensor(const std::string& prefPrefix): Sensor(prefPrefix) {}

    virtual ~TemperatureSensor() = default;
};

extern std::unique_ptr<TemperatureSensor> CreateTemperatureSensor(
    App& app,
    const std::string& prefPrefix,
    const std::string& hwmonRoot = "/sys/class/hwmon");
