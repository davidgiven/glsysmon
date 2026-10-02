#pragma once

#include "sensors/sensors.h"

class MockSensors : public Sensors
{
public:
    explicit MockSensors(Timer& timer): _timer(timer) {}

    std::unique_ptr<ClockSensor> CreateClockSensor(
        const std::string& prefPrefix) const override
    {
        (void)prefPrefix;
        return nullptr;
    }

    std::unique_ptr<CpuSensor> CreateCpuSensor(const std::string& prefPrefix,
        const std::string& procStatPath) const override
    {
        (void)prefPrefix;
        (void)procStatPath;
        return nullptr;
    }

    std::unique_ptr<HostnameSensor> CreateHostnameSensor(
        const std::string& prefPrefix) const override
    {
        (void)prefPrefix;
        return nullptr;
    }

    std::unique_ptr<TemperatureSensor> CreateTemperatureSensor(
        const std::string& prefPrefix,
        const std::string& hwmonRoot) const override
    {
        (void)prefPrefix;
        (void)hwmonRoot;
        return nullptr;
    }

    std::unique_ptr<NetworkSensor> CreateNetworkSensor(
        const std::string& prefPrefix,
        const std::string& procNetDevPath) const override
    {
        (void)prefPrefix;
        (void)procNetDevPath;
        return nullptr;
    }

    std::unique_ptr<DiskSensor> CreateDiskSensor(const std::string& prefPrefix,
        const std::string& procDiskStatsPath) const override
    {
        (void)prefPrefix;
        (void)procDiskStatsPath;
        return nullptr;
    }

    std::unique_ptr<MemorySensor> CreateMemorySensor(
        const std::string& prefPrefix,
        const std::string& procMemInfoPath) const override
    {
        (void)prefPrefix;
        (void)procMemInfoPath;
        return nullptr;
    }

    std::shared_ptr<NetworkPoller> CreateNetworkPoller(
        const std::string& procNetDevPath) const override
    {
        (void)procNetDevPath;
        return nullptr;
    }

    std::shared_ptr<MemoryPoller> CreateMemoryPoller(
        const std::string& procMemInfoPath) const override
    {
        (void)procMemInfoPath;
        return nullptr;
    }

    std::shared_ptr<CpuPoller> CreateCpuPoller(
        const std::string& procStatPath) const override
    {
        (void)procStatPath;
        return nullptr;
    }

    Timer& GetTimer() const override
    {
        return _timer;
    }

private:
    Timer& _timer;
};