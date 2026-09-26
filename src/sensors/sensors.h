#pragma once

#include <memory>
#include <string>
#include <vector>

#include "clock_sensor.h"
#include "cpu_sensor.h"
#include "disk_sensor.h"
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
    virtual ~Sensors() = default;

    Sensors(const Sensors&) = delete;
    Sensors& operator=(const Sensors&) = delete;

    virtual std::unique_ptr<ClockSensor> CreateClockSensor(
        const std::string& prefPrefix) const;
    virtual std::unique_ptr<CpuSensor> CreateCpuSensor(
        const std::string& prefPrefix,
        const std::string& procStatPath = "/proc/stat") const;
    virtual std::unique_ptr<HostnameSensor> CreateHostnameSensor(
        const std::string& prefPrefix) const;
    virtual std::unique_ptr<TemperatureSensor> CreateTemperatureSensor(
        const std::string& prefPrefix,
        const std::string& hwmonRoot = "/sys/class/hwmon") const;
    virtual std::unique_ptr<NetworkSensor> CreateNetworkSensor(
        const std::string& prefPrefix,
        const std::string& procNetDevPath = "/proc/net/dev") const;
    virtual std::unique_ptr<DiskSensor> CreateDiskSensor(
        const std::string& prefPrefix,
        const std::string& procDiskStatsPath = "/proc/diskstats") const;

private:
    const Preferences& _prefs;
    Timer& _timer;
};
