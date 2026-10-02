#pragma once

#include "app.h"

#include "cpu_poller.h"
#include "sensor.h"
#include "sensor_graph_mixin.h"

#include <memory>
#include <string>

class App;

class CpuSensor : public Sensor, public SensorGraphMixin<CpuSample>
{
public:
    explicit CpuSensor(App& app, const std::string& prefPrefix):
        Sensor(app, prefPrefix)
    {
    }

    explicit CpuSensor(const std::string& prefPrefix): Sensor(prefPrefix) {}

    virtual ~CpuSensor() = default;
};

extern std::unique_ptr<CpuSensor> CreateCpuSensor(App& app,
    const std::string& prefPrefix,
    std::shared_ptr<CpuPoller> poller);
