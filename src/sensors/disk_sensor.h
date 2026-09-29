#pragma once

#include "sensor.h"
#include "sensor_rx_tx_graph_mixin.h"

#include <memory>
#include <string>

class Context;

using DiskSample = RxTxSample;

class DiskSensor : public Sensor, public SensorRxTxGraphMixin
{
public:
    explicit DiskSensor(const Context& ctx, const std::string& prefPrefix):
        Sensor(ctx, prefPrefix)
    {
    }

    explicit DiskSensor(const std::string& prefPrefix): Sensor(prefPrefix) {}

    virtual ~DiskSensor() = default;
};

extern std::unique_ptr<DiskSensor> CreateDiskSensor(const Context& ctx,
    const std::string& prefPrefix,
    const std::string& procDiskStatsPath = "/proc/diskstats");
