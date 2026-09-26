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

#include "preferences/preferences.h"
#include "sensor_graph_mixin.h"
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
        explicit DiskSensorImpl(const Preferences& prefs,
            Timer& timer,
            const std::string& prefPrefix,
            const std::string& procDiskStatsPath):
            DiskSensor(prefPrefix),
            _timer(timer),
            _procDiskStatsPath(procDiskStatsPath)
        {
            int size = GlobalPreferencesFetcher::GetSize(prefs);
            if (size <= 0)
                size = 1;
            std::size_t sampleCount = static_cast<std::size_t>(size);
            double interval =
                prefs.GetDouble(_prefPrefix + ".update_interval").value_or(1);
            if (interval <= 0)
                interval = 1;
            _interval = interval;
            _delta = static_cast<std::uint64_t>(1'000'000'000ULL / interval);

            _deviceNames = DiscoverDevices(_procDiskStatsPath);
            InitGraph(_deviceNames.size(), sampleCount, DiskSample{});
            _prev.resize(_deviceNames.size());
            Tick(_timer.Now());
        }

        const DiskSample* GetSamples(std::size_t channel) const override
        {
            if (channel >= GetChannels())
                return nullptr;
            return this->SensorGraphMixin<DiskSample>::GetSamples(channel);
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
            float interval = static_cast<float>(
                preferences.GetDouble(_prefPrefix + ".update_interval")
                    .value_or(2.0));
            if (ImGui::InputFloat("Update interval (Hz)", &interval))
            {
                preferences.SetDouble(
                    _prefPrefix + ".update_interval", interval);
            }
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
                _timer.Schedule(t + _delta,
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
                                512.0 * _interval;
                    if (now.wrSectors >= prev.wrSectors)
                        txBps = static_cast<double>(
                                    now.wrSectors - prev.wrSectors) *
                                512.0 * _interval;
                    DiskSample s{txBps, rxBps};
                    AddSample(i, s);
                }
                _prev = cur;
            }
            _timer.Schedule(t + _delta,
                std::bind(&DiskSensorImpl::Tick, this, std::placeholders::_1));
        }

        void PushZeros()
        {
            DiskSample zero{0, 0};
            for (std::size_t i = 0; i < GetChannels(); ++i)
                AddSample(i, zero);
        }

        Timer& _timer;
        std::string _procDiskStatsPath;
        std::vector<std::string> _deviceNames;
        std::vector<RawDisk> _prev;
        bool _first = true;
        std::uint64_t _delta = 0;
        double _interval = 1;
    };

} // namespace

std::unique_ptr<DiskSensor> CreateDiskSensor(const Preferences& prefs,
    Timer& timer,
    const std::string& prefPrefix,
    const std::string& procDiskStatsPath)
{
    return std::make_unique<DiskSensorImpl>(
        prefs, timer, prefPrefix, procDiskStatsPath);
}
