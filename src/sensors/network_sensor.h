#pragma once

#include "sensor.h"
#include "sensor_rx_tx_graph_mixin.h"

#include <memory>
#include <string>

class NetworkPoller;
class Context;

using NetworkSample = RxTxSample;

class NetworkSensor : public Sensor, public SensorRxTxGraphMixin
{
public:
    explicit NetworkSensor(const Context& ctx, const std::string& prefPrefix):
        Sensor(ctx, prefPrefix)
    {
    }

    explicit NetworkSensor(const std::string& prefPrefix): Sensor(prefPrefix) {}

    virtual ~NetworkSensor() = default;
};

extern std::unique_ptr<NetworkSensor> CreateNetworkSensor(const Context& ctx,
    const std::string& prefPrefix,
    const std::string& procNetDevPath = "/proc/net/dev");

extern std::unique_ptr<NetworkSensor> CreateNetworkSensor(const Context& ctx,
    const std::string& prefPrefix,
    std::shared_ptr<NetworkPoller> poller);
