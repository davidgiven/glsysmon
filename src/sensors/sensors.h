#pragma once

#include <memory>
#include <string>
#include <vector>

#include "clock_sensor.h"
#include "cpu_poller.h"
#include "cpu_sensor.h"
#include "disk_sensor.h"
#include "hostname_sensor.h"
#include "memory_poller.h"
#include "memory_sensor.h"
#include "network_poller.h"
#include "network_sensor.h"
#include "temperature_sensor.h"

class Context;
class Timer;

class Sensors
{
public:
    virtual ~Sensors() = default;

    Sensors(const Sensors&) = delete;
    Sensors& operator=(const Sensors&) = delete;

    virtual std::unique_ptr<ClockSensor> CreateClockSensor(
        const std::string& prefPrefix) const = 0;
    virtual std::unique_ptr<CpuSensor> CreateCpuSensor(
        const std::string& prefPrefix,
        const std::string& procStatPath = "/proc/stat") const = 0;
    virtual std::unique_ptr<HostnameSensor> CreateHostnameSensor(
        const std::string& prefPrefix) const = 0;
    virtual std::unique_ptr<TemperatureSensor> CreateTemperatureSensor(
        const std::string& prefPrefix,
        const std::string& hwmonRoot = "/sys/class/hwmon") const = 0;
    virtual std::unique_ptr<NetworkSensor> CreateNetworkSensor(
        const std::string& prefPrefix,
        const std::string& procNetDevPath = "/proc/net/dev") const = 0;
    virtual std::unique_ptr<DiskSensor> CreateDiskSensor(
        const std::string& prefPrefix,
        const std::string& procDiskStatsPath = "/proc/diskstats") const = 0;
    virtual std::unique_ptr<MemorySensor> CreateMemorySensor(
        const std::string& prefPrefix,
        const std::string& procMemInfoPath = "/proc/meminfo") const = 0;
    virtual std::shared_ptr<NetworkPoller> CreateNetworkPoller(
        const std::string& procNetDevPath = "/proc/net/dev") const = 0;
    virtual std::shared_ptr<MemoryPoller> CreateMemoryPoller(
        const std::string& procMemInfoPath = "/proc/meminfo") const = 0;
    virtual std::shared_ptr<CpuPoller> CreateCpuPoller(
        const std::string& procStatPath = "/proc/stat") const = 0;

    virtual Timer& GetTimer() const = 0;

protected:
    Sensors() = default;
};

extern std::unique_ptr<Sensors> CreateSensors(const Context& ctx);
