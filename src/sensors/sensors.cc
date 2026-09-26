#include "sensors.h"

#include <vector>

#include "preferences/preferences.h"
#include "sensors/clock_sensor.h"
#include "sensors/cpu_poller.h"
#include "sensors/cpu_sensor.h"
#include "sensors/disk_sensor.h"
#include "sensors/hostname_sensor.h"
#include "sensors/memory_poller.h"
#include "sensors/network_poller.h"
#include "sensors/network_sensor.h"
#include "sensors/temperature_sensor.h"

Sensors::Sensors(const Preferences& prefs, Timer& timer):
    _prefs(prefs),
    _timer(timer)
{
}

std::unique_ptr<ClockSensor> Sensors::CreateClockSensor(
    const std::string& prefPrefix) const
{
    return ::CreateClockSensor(_prefs, _timer, prefPrefix);
}

std::unique_ptr<CpuSensor> Sensors::CreateCpuSensor(
    const std::string& a, const std::string& b) const
{
    auto poller = CreateCpuPoller(b);
    return ::CreateCpuSensor(_prefs, _timer, a, poller);
}

std::unique_ptr<HostnameSensor> Sensors::CreateHostnameSensor(
    const std::string& prefPrefix) const
{
    return ::CreateHostnameSensor(_prefs, _timer, prefPrefix);
}

std::unique_ptr<TemperatureSensor> Sensors::CreateTemperatureSensor(
    const std::string& a, const std::string& b) const
{
    return ::CreateTemperatureSensor(_prefs, _timer, a, b);
}

std::unique_ptr<NetworkSensor> Sensors::CreateNetworkSensor(
    const std::string& a, const std::string& b) const
{
    auto poller = CreateNetworkPoller(b);
    return ::CreateNetworkSensor(_prefs, _timer, a, poller);
}

std::unique_ptr<DiskSensor> Sensors::CreateDiskSensor(
    const std::string& a, const std::string& b) const
{
    return ::CreateDiskSensor(_prefs, _timer, a, b);
}

std::shared_ptr<NetworkPoller> Sensors::CreateNetworkPoller(
    const std::string& procNetDevPath) const
{
    if (!_networkPoller)
    {
        _networkPoller = ::CreateNetworkPoller(procNetDevPath);
        _networkPoller->SetCacheIntervalMs(
            GlobalPreferencesFetcher::GetPollerCacheInterval(_prefs));
    }
    else
    {
        _networkPoller->SetCacheIntervalMs(
            GlobalPreferencesFetcher::GetPollerCacheInterval(_prefs));
    }
    return _networkPoller;
}

std::shared_ptr<MemoryPoller> Sensors::CreateMemoryPoller(
    const std::string& procMemInfoPath) const
{
    if (!_memoryPoller)
    {
        _memoryPoller = ::CreateMemoryPoller(procMemInfoPath);
        _memoryPoller->SetCacheIntervalMs(
            GlobalPreferencesFetcher::GetPollerCacheInterval(_prefs));
    }
    else
    {
        _memoryPoller->SetCacheIntervalMs(
            GlobalPreferencesFetcher::GetPollerCacheInterval(_prefs));
    }
    return _memoryPoller;
}

std::shared_ptr<CpuPoller> Sensors::CreateCpuPoller(
    const std::string& procStatPath) const
{
    if (!_cpuPoller)
    {
        _cpuPoller = ::CreateCpuPoller(procStatPath);
        _cpuPoller->SetCacheIntervalMs(
            GlobalPreferencesFetcher::GetPollerCacheInterval(_prefs));
    }
    else
    {
        _cpuPoller->SetCacheIntervalMs(
            GlobalPreferencesFetcher::GetPollerCacheInterval(_prefs));
    }
    return _cpuPoller;
}
