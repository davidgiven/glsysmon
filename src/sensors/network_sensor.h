#pragma once

#include "app.h"

#include "sensor.h"
#include "sensor_rx_tx_graph_mixin.h"

#include <memory>
#include <string>

class NetworkPoller;
class App;

using NetworkSample = RxTxSample;

class NetworkSensor : public Sensor, public SensorRxTxGraphMixin
{
public:
    explicit NetworkSensor(App& app, const std::string& prefPrefix):
        Sensor(app, prefPrefix)
    {
    }

    explicit NetworkSensor(const std::string& prefPrefix): Sensor(prefPrefix) {}

    virtual ~NetworkSensor() = default;
};

extern std::unique_ptr<NetworkSensor> CreateNetworkSensor(App& app,
    const std::string& prefPrefix,
    const std::string& procNetDevPath = "/proc/net/dev");

extern std::unique_ptr<NetworkSensor> CreateNetworkSensor(App& app,
    const std::string& prefPrefix,
    std::shared_ptr<NetworkPoller> poller);
