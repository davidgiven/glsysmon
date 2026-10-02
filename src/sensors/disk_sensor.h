#pragma once

#include "app.h"

#include "sensor.h"
#include "sensor_rx_tx_graph_mixin.h"

#include <memory>
#include <string>

class App;

using DiskSample = RxTxSample;

class DiskSensor : public Sensor, public SensorRxTxGraphMixin
{
public:
    explicit DiskSensor(App& app, const std::string& prefPrefix):
        Sensor(app, prefPrefix)
    {
    }

    explicit DiskSensor(const std::string& prefPrefix): Sensor(prefPrefix) {}

    virtual ~DiskSensor() = default;
};

extern std::unique_ptr<DiskSensor> CreateDiskSensor(App& app,
    const std::string& prefPrefix,
    const std::string& procDiskStatsPath = "/proc/diskstats");
