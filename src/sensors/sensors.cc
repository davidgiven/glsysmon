#include "app.h"
#include "sensors.h"

#include "preferences/preferences.h"
#include "sensors/clock_sensor.h"
#include "sensors/cpu_poller.h"
#include "sensors/cpu_sensor.h"
#include "sensors/disk_sensor.h"
#include "sensors/hostname_sensor.h"
#include "sensors/memory_poller.h"
#include "sensors/memory_sensor.h"
#include "sensors/network_poller.h"
#include "sensors/network_sensor.h"
#include "sensors/temperature_sensor.h"

namespace
{

    class SensorsImpl final : public Sensors
    {
    public:
        explicit SensorsImpl(App& app): _app(app) {}

        std::unique_ptr<ClockSensor> CreateClockSensor(
            const std::string& prefPrefix) const override
        {
            return ::CreateClockSensor(_app, prefPrefix);
        }

        std::unique_ptr<CpuSensor> CreateCpuSensor(
            const std::string& prefPrefix,
            const std::string& procStatPath) const override
        {
            auto poller = CreateCpuPoller(procStatPath);
            return ::CreateCpuSensor(_app, prefPrefix, poller);
        }

        std::unique_ptr<HostnameSensor> CreateHostnameSensor(
            const std::string& prefPrefix) const override
        {
            return ::CreateHostnameSensor(_app, prefPrefix);
        }

        std::unique_ptr<TemperatureSensor> CreateTemperatureSensor(
            const std::string& prefPrefix,
            const std::string& hwmonRoot) const override
        {
            return ::CreateTemperatureSensor(_app, prefPrefix, hwmonRoot);
        }

        std::unique_ptr<NetworkSensor> CreateNetworkSensor(
            const std::string& prefPrefix,
            const std::string& procNetDevPath) const override
        {
            auto poller = CreateNetworkPoller(procNetDevPath);
            return ::CreateNetworkSensor(_app, prefPrefix, poller);
        }

        std::unique_ptr<DiskSensor> CreateDiskSensor(
            const std::string& prefPrefix,
            const std::string& procDiskStatsPath) const override
        {
            return ::CreateDiskSensor(_app, prefPrefix, procDiskStatsPath);
        }

        std::unique_ptr<MemorySensor> CreateMemorySensor(
            const std::string& prefPrefix,
            const std::string& procMemInfoPath) const override
        {
            auto poller = CreateMemoryPoller(procMemInfoPath);
            return ::CreateMemorySensor(_app, prefPrefix, poller);
        }

        std::shared_ptr<NetworkPoller> CreateNetworkPoller(
            const std::string& procNetDevPath) const override
        {
            if (!_networkPoller)
                _networkPoller = ::CreateNetworkPoller(procNetDevPath);
            _networkPoller->SetCacheIntervalMs(
                GlobalPreferencesFetcher::GetPollerCacheInterval(
                    _app.GetPreferences()));
            return _networkPoller;
        }

        std::shared_ptr<MemoryPoller> CreateMemoryPoller(
            const std::string& procMemInfoPath) const override
        {
            if (!_memoryPoller)
                _memoryPoller = ::CreateMemoryPoller(procMemInfoPath);
            _memoryPoller->SetCacheIntervalMs(
                GlobalPreferencesFetcher::GetPollerCacheInterval(
                    _app.GetPreferences()));
            return _memoryPoller;
        }

        std::shared_ptr<CpuPoller> CreateCpuPoller(
            const std::string& procStatPath) const override
        {
            if (!_cpuPoller)
                _cpuPoller = ::CreateCpuPoller(procStatPath);
            _cpuPoller->SetCacheIntervalMs(
                GlobalPreferencesFetcher::GetPollerCacheInterval(
                    _app.GetPreferences()));
            return _cpuPoller;
        }

        Timer& GetTimer() const override
        {
            return _app.GetTimer();
        }

    private:
        App& _app;
        mutable std::shared_ptr<NetworkPoller> _networkPoller;
        mutable std::shared_ptr<MemoryPoller> _memoryPoller;
        mutable std::shared_ptr<CpuPoller> _cpuPoller;
    };

} // namespace

std::unique_ptr<Sensors> CreateSensors(App& app)
{
    return std::make_unique<SensorsImpl>(app);
}
