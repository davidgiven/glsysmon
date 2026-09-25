#pragma once

#include <memory>
#include <string>
#include <vector>

#include "clock_sensor.h"
#include "cpu_sensor.h"
#include "hostname_sensor.h"
#include "network_sensor.h"
#include "temperature_sensor.h"

class Preferences;
class Sensor;
class Timer;

class Sensors
{
public:
    explicit Sensors(const Preferences& prefs, Timer& timer);

    Sensors(const Sensors&) = delete;
    Sensors& operator=(const Sensors&) = delete;

    std::unique_ptr<ClockSensor> CreateClockSensor(
        const std::string& prefPrefix) const;
    std::unique_ptr<CpuSensor> CreateCpuSensor(const std::string& prefPrefix,
        const std::string& procStatPath = "/proc/stat") const;
    std::unique_ptr<HostnameSensor> CreateHostnameSensor(
        const std::string& prefPrefix) const;
    std::unique_ptr<TemperatureSensor> CreateTemperatureSensor(
        const std::string& prefPrefix,
        const std::string& hwmonRoot = "/sys/class/hwmon") const;
    std::unique_ptr<NetworkSensor> CreateNetworkSensor(
        const std::string& prefPrefix,
        const std::string& procNetDevPath = "/proc/net/dev") const;

private:
    const Preferences& _prefs;
    Timer& _timer;
};
