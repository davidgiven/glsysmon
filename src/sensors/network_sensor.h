#pragma once

#include "sensor.h"
#include "sensor_graph_mixin.h"

#include <memory>
#include <string>

class Preferences;
class Timer;

struct NetworkSample
{
    double txBps;
    double rxBps;
};

class NetworkSensor : public Sensor, public SensorGraphMixin<NetworkSample>
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
