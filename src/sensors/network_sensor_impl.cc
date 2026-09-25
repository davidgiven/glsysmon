#include "network_sensor.h"

#include <imgui.h>

#include <algorithm>
#include <cctype>
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

    std::string Trim(const std::string& s)
    {
        std::size_t start = 0;
        while (start < s.size() &&
               std::isspace(static_cast<unsigned char>(s[start])))
            start++;
        std::size_t end = s.size();
        while (
            end > start && std::isspace(static_cast<unsigned char>(s[end - 1])))
            end--;
        return s.substr(start, end - start);
    }

    struct RawNet
    {
        std::uint64_t rxBytes = 0;
        std::uint64_t txBytes = 0;
    };

    class NetworkSensorImpl : public NetworkSensor
    {
    public:
        explicit NetworkSensorImpl(const Preferences& prefs,
            Timer& timer,
            const std::string& prefPrefix,
            const std::string& procNetDevPath):
            NetworkSensor(prefPrefix),
            _timer(timer),
            _procNetDevPath(procNetDevPath)
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

            _ifaceNames = DiscoverInterfaces(_procNetDevPath);
            InitGraph(_ifaceNames.size(), sampleCount, NetworkSample{});
            _prev.resize(_ifaceNames.size());
            Tick(_timer.Now());
        }

        const NetworkSample* GetSamples(std::size_t channel) const override
        {
            if (channel >= GetChannels())
                return nullptr;
            return this->SensorGraphMixin<NetworkSample>::GetSamples(channel);
        }

        std::string GetChannelName(std::size_t channel) const override
        {
            if (channel < _ifaceNames.size())
                return _ifaceNames[channel];
            return std::to_string(channel);
        }

        std::string GetHumanName() const override
        {
            return "Network";
        }

        std::string GetPrefName() const override
        {
            return "network";
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
        static std::vector<std::string> DiscoverInterfaces(
            const std::string& path)
        {
            std::ifstream file(path);
            if (!file)
                return {};
            std::string line;
            std::getline(file, line);
            std::getline(file, line);
            std::vector<std::string> names;
            while (std::getline(file, line))
            {
                std::size_t colon = line.find(':');
                if (colon == std::string::npos)
                    continue;
                std::string iface = Trim(line.substr(0, colon));
                if (!iface.empty())
                    names.push_back(iface);
            }
            std::sort(names.begin(), names.end());
            return names;
        }

        static std::vector<RawNet> ReadCurrent(
            const std::string& path, const std::vector<std::string>& names)
        {
            std::vector<RawNet> result;
            result.reserve(names.size());
            std::ifstream file(path);
            if (!file)
            {
                result.assign(names.size(), RawNet{});
                return result;
            }
            std::string line;
            std::getline(file, line);
            std::getline(file, line);
            std::unordered_map<std::string, RawNet> map;
            while (std::getline(file, line))
            {
                std::size_t colon = line.find(':');
                if (colon == std::string::npos)
                    continue;
                std::string iface = Trim(line.substr(0, colon));
                std::string data = line.substr(colon + 1);
                std::istringstream iss(data);
                std::uint64_t rxBytes = 0;
                std::uint64_t rxPackets = 0;
                std::uint64_t rxErrs = 0;
                std::uint64_t rxDrop = 0;
                std::uint64_t rxFifo = 0;
                std::uint64_t rxFrame = 0;
                std::uint64_t rxCompressed = 0;
                std::uint64_t rxMulticast = 0;
                std::uint64_t txBytes = 0;
                iss >> rxBytes >> rxPackets >> rxErrs >> rxDrop >> rxFifo >>
                    rxFrame >> rxCompressed >> rxMulticast >> txBytes;
                if (iss)
                    map[iface] = RawNet{rxBytes, txBytes};
            }
            for (const auto& n : names)
            {
                auto it = map.find(n);
                if (it != map.end())
                    result.push_back(it->second);
                else
                    result.push_back(RawNet{});
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
                        &NetworkSensorImpl::Tick, this, std::placeholders::_1));
                return;
            }

            std::vector<RawNet> cur = ReadCurrent(_procNetDevPath, _ifaceNames);
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
                    const RawNet& prev = _prev[i];
                    const RawNet& now = cur[i];
                    double rxBps = 0;
                    double txBps = 0;
                    if (now.rxBytes >= prev.rxBytes)
                        rxBps =
                            static_cast<double>(now.rxBytes - prev.rxBytes) *
                            _interval;
                    if (now.txBytes >= prev.txBytes)
                        txBps =
                            static_cast<double>(now.txBytes - prev.txBytes) *
                            _interval;
                    NetworkSample s{txBps, rxBps};
                    AddSample(i, s);
                }
                _prev = cur;
            }
            _timer.Schedule(t + _delta,
                std::bind(
                    &NetworkSensorImpl::Tick, this, std::placeholders::_1));
        }

        void PushZeros()
        {
            NetworkSample zero{0, 0};
            for (std::size_t i = 0; i < GetChannels(); ++i)
                AddSample(i, zero);
        }

        Timer& _timer;
        std::string _procNetDevPath;
        std::vector<std::string> _ifaceNames;
        std::vector<RawNet> _prev;
        bool _first = true;
        std::uint64_t _delta = 0;
        double _interval = 1;
    };

} // namespace

std::unique_ptr<NetworkSensor> CreateNetworkSensor(const Preferences& prefs,
    Timer& timer,
    const std::string& prefPrefix,
    const std::string& procNetDevPath)
{
    return std::make_unique<NetworkSensorImpl>(
        prefs, timer, prefPrefix, procNetDevPath);
}
