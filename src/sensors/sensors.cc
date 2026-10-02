#include "sensors.h"

#include "context.h"
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
        explicit SensorsImpl(const Context& ctx): _ctx(ctx) {}

        std::unique_ptr<ClockSensor> CreateClockSensor(
            const std::string& prefPrefix) const override
        {
            return ::CreateClockSensor(_ctx, prefPrefix);
        }

        std::unique_ptr<CpuSensor> CreateCpuSensor(
            const std::string& prefPrefix,
            const std::string& procStatPath) const override
        {
            auto poller = CreateCpuPoller(procStatPath);
            return ::CreateCpuSensor(_ctx, prefPrefix, poller);
        }

        std::unique_ptr<HostnameSensor> CreateHostnameSensor(
            const std::string& prefPrefix) const override
        {
            return ::CreateHostnameSensor(_ctx, prefPrefix);
        }

        std::unique_ptr<TemperatureSensor> CreateTemperatureSensor(
            const std::string& prefPrefix,
            const std::string& hwmonRoot) const override
        {
            return ::CreateTemperatureSensor(_ctx, prefPrefix, hwmonRoot);
        }

        std::unique_ptr<NetworkSensor> CreateNetworkSensor(
            const std::string& prefPrefix,
            const std::string& procNetDevPath) const override
        {
            auto poller = CreateNetworkPoller(procNetDevPath);
            return ::CreateNetworkSensor(_ctx, prefPrefix, poller);
        }

        std::unique_ptr<DiskSensor> CreateDiskSensor(
            const std::string& prefPrefix,
            const std::string& procDiskStatsPath) const override
        {
            return ::CreateDiskSensor(_ctx, prefPrefix, procDiskStatsPath);
        }

        std::unique_ptr<MemorySensor> CreateMemorySensor(
            const std::string& prefPrefix,
            const std::string& procMemInfoPath) const override
        {
            auto poller = CreateMemoryPoller(procMemInfoPath);
            return ::CreateMemorySensor(_ctx, prefPrefix, poller);
        }

        std::shared_ptr<NetworkPoller> CreateNetworkPoller(
            const std::string& procNetDevPath) const override
        {
            if (!_networkPoller)
                _networkPoller = ::CreateNetworkPoller(procNetDevPath);
            _networkPoller->SetCacheIntervalMs(
                GlobalPreferencesFetcher::GetPollerCacheInterval(
                    _ctx.preferences));
            return _networkPoller;
        }

        std::shared_ptr<MemoryPoller> CreateMemoryPoller(
            const std::string& procMemInfoPath) const override
        {
            if (!_memoryPoller)
                _memoryPoller = ::CreateMemoryPoller(procMemInfoPath);
            _memoryPoller->SetCacheIntervalMs(
                GlobalPreferencesFetcher::GetPollerCacheInterval(
                    _ctx.preferences));
            return _memoryPoller;
        }

        std::shared_ptr<CpuPoller> CreateCpuPoller(
            const std::string& procStatPath) const override
        {
            if (!_cpuPoller)
                _cpuPoller = ::CreateCpuPoller(procStatPath);
            _cpuPoller->SetCacheIntervalMs(
                GlobalPreferencesFetcher::GetPollerCacheInterval(
                    _ctx.preferences));
            return _cpuPoller;
        }

        Timer& GetTimer() const override
        {
            return _ctx.timer;
        }

    private:
        const Context& _ctx;
        mutable std::shared_ptr<NetworkPoller> _networkPoller;
        mutable std::shared_ptr<MemoryPoller> _memoryPoller;
        mutable std::shared_ptr<CpuPoller> _cpuPoller;
    };

} // namespace

std::unique_ptr<Sensors> CreateSensors(const Context& ctx)
{
    return std::make_unique<SensorsImpl>(ctx);
}
