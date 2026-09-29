#include "disk_sensor.h"

#include <imgui.h>

#include <algorithm>
#include <cstdint>
#include <fstream>
#include <functional>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "context.h"
#include "preferences/preferences.h"
#include "sensor_rx_tx_graph_mixin.h"
#include "timer.h"

namespace
{

    struct RawDisk
    {
        std::uint64_t rdSectors = 0;
        std::uint64_t wrSectors = 0;
    };

    class DiskSensorImpl : public DiskSensor
    {
    public:
        explicit DiskSensorImpl(const Context& ctx,
            const std::string& prefPrefix,
            const std::string& procDiskStatsPath): DiskSensor(ctx, prefPrefix),
            _ctx(ctx),
            _procDiskStatsPath(procDiskStatsPath)
        {
            _deviceNames = DiscoverDevices(_procDiskStatsPath);
            InitGraph(_ctx,
                _prefPrefix,
                _deviceNames.size(),
                RxTxSample{});
            _prev.resize(_deviceNames.size());
            Tick(_ctx.timer.Now());
        }

        const RxTxSample* GetSamples(std::size_t channel) const override
        {
            if (channel >= GetChannels())
                return nullptr;
            return this->SensorRxTxGraphMixin::GetSamples(channel);
        }

        std::string GetChannelName(std::size_t channel) const override
        {
            if (channel < _deviceNames.size())
                return _deviceNames[channel];
            return std::to_string(channel);
        }

        std::string GetHumanName() const override
        {
            return "Disk";
        }

        std::string GetPrefName() const override
        {
            return "disk";
        }

        void DrawConfiguration(Preferences& preferences) override
        {
            DrawIntervalConfiguration(preferences, _prefPrefix);
        }

    private:
        static std::vector<std::string> DiscoverDevices(const std::string& path)
        {
            std::ifstream file(path);
            if (!file)
                return {};
            std::string line;
            std::vector<std::string> names;
            while (std::getline(file, line))
            {
                std::istringstream iss(line);
                std::uint64_t major = 0;
                std::uint64_t minor = 0;
                std::string name;
                if (!(iss >> major >> minor >> name))
                    continue;
                if (!name.empty())
                    names.push_back(name);
            }
            std::sort(names.begin(), names.end());
            return names;
        }

        static std::vector<RawDisk> ReadCurrent(
            const std::string& path, const std::vector<std::string>& names)
        {
            std::vector<RawDisk> result;
            result.reserve(names.size());
            std::ifstream file(path);
            if (!file)
            {
                result.assign(names.size(), RawDisk{});
                return result;
            }
            std::unordered_map<std::string, RawDisk> map;
            std::string line;
            while (std::getline(file, line))
            {
                std::istringstream iss(line);
                std::uint64_t major = 0;
                std::uint64_t minor = 0;
                std::string name;
                std::uint64_t rdCompleted = 0;
                std::uint64_t rdMerged = 0;
                std::uint64_t rdSectors = 0;
                std::uint64_t rdTicks = 0;
                std::uint64_t wrCompleted = 0;
                std::uint64_t wrMerged = 0;
                std::uint64_t wrSectors = 0;
                if (!(iss >> major >> minor >> name >> rdCompleted >>
                        rdMerged >> rdSectors >> rdTicks >> wrCompleted >>
                        wrMerged >> wrSectors))
                    continue;
                map[name] = RawDisk{rdSectors, wrSectors};
            }
            for (const auto& n : names)
            {
                auto it = map.find(n);
                if (it != map.end())
                    result.push_back(it->second);
                else
                    result.push_back(RawDisk{});
            }
            return result;
        }

        void Tick(Timer::Time t)
        {
            std::size_t channels = GetChannels();
            if (channels == 0)
            {
                _ctx.timer.Schedule(t + this->_delta,
                    std::bind(
                        &DiskSensorImpl::Tick, this, std::placeholders::_1));
                return;
            }

            std::vector<RawDisk> cur =
                ReadCurrent(_procDiskStatsPath, _deviceNames);
            if (cur.size() != channels)
            {
                PushZeros();
            }
            else if (_first)
            {
                _prev = cur;
                _first = false;
                PushZeros();
            }
            else
            {
                for (std::size_t i = 0; i < channels; ++i)
                {
                    const RawDisk& prev = _prev[i];
                    const RawDisk& now = cur[i];
                    double rxBps = 0;
                    double txBps = 0;
                    if (now.rdSectors >= prev.rdSectors)
                        rxBps = static_cast<double>(
                                    now.rdSectors - prev.rdSectors) *
                                512.0 * this->_interval;
                    if (now.wrSectors >= prev.wrSectors)
                        txBps = static_cast<double>(
                                    now.wrSectors - prev.wrSectors) *
                                512.0 * this->_interval;
                    RxTxSample s{txBps, rxBps};
                    AddSample(i, s);
                }
                _prev = cur;
            }
            _ctx.timer.Schedule(t + this->_delta,
                std::bind(&DiskSensorImpl::Tick, this, std::placeholders::_1));
        }

        void PushZeros()
        {
            RxTxSample zero{0, 0};
            for (std::size_t i = 0; i < GetChannels(); ++i)
                AddSample(i, zero);
        }

        const Context& _ctx;
        std::string _procDiskStatsPath;
        std::vector<std::string> _deviceNames;
        std::vector<RawDisk> _prev;
        bool _first = true;
    };

} // namespace

std::unique_ptr<DiskSensor> CreateDiskSensor(const Context& ctx,
    const std::string& prefPrefix,
    const std::string& procDiskStatsPath)
{
    return std::make_unique<DiskSensorImpl>(ctx, prefPrefix, procDiskStatsPath);
}
