#pragma once

#include "sensor.h"
#include "sensor_rx_tx_graph_mixin.h"

#include <memory>
#include <string>

class Preferences;
class Timer;

using NetworkSample = RxTxSample;

class NetworkSensor : public Sensor, public SensorRxTxGraphMixin
{
public:
    explicit NetworkSensor(const std::string& prefPrefix): Sensor(prefPrefix) {}

    virtual ~NetworkSensor() = default;
};

extern std::unique_ptr<NetworkSensor> CreateNetworkSensor(
    const Preferences& prefs,
    Timer& timer,
    const std::string& prefPrefix,
    const std::string& procNetDevPath = "/proc/net/dev");
