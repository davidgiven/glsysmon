#include "app.h"
#include "network_poller.h"

#include "utils.h"

#include <algorithm>
#include <cstdint>
#include <fstream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>

namespace
{

    struct RawNet
    {
        std::uint64_t rxBytes = 0;
        std::uint64_t txBytes = 0;
    };

    class NetworkPollerImpl : public NetworkPoller
    {
    public:
        explicit NetworkPollerImpl(const std::string& procNetDevPath):
            _procNetDevPath(procNetDevPath)
        {
        }

        std::map<std::string, RxTxSample> Poll() override
        {
            std::map<std::string, RawNet> cur = ReadRaw(_procNetDevPath);
            std::map<std::string, RxTxSample> result;

            if (_first)
            {
                _first = false;
                _prev = cur;
                for (const auto& kv : cur)
                    result[kv.first] = RxTxSample{0, 0};
                return result;
            }

            for (const auto& kv : cur)
            {
                const std::string& iface = kv.first;
                const RawNet& now = kv.second;
                auto it = _prev.find(iface);
                if (it == _prev.end())
                {
                    result[iface] = RxTxSample{0, 0};
                    continue;
                }
                const RawNet& prev = it->second;
                double rxBps = 0;
                double txBps = 0;
                if (now.rxBytes >= prev.rxBytes)
                    rxBps = static_cast<double>(now.rxBytes - prev.rxBytes);
                if (now.txBytes >= prev.txBytes)
                    txBps = static_cast<double>(now.txBytes - prev.txBytes);
                result[iface] = RxTxSample{txBps, rxBps};
            }

            _prev = cur;
            return result;
        }

    private:
        static std::map<std::string, RawNet> ReadRaw(const std::string& path)
        {
            std::map<std::string, RawNet> map;
            std::ifstream file(path);
            if (!file)
                return map;
            std::string line;
            std::getline(file, line);
            std::getline(file, line);
            while (std::getline(file, line))
            {
                std::size_t colon = line.find(':');
                if (colon == std::string::npos)
                    continue;
                std::string iface = Trim(line.substr(0, colon));
                if (iface.empty())
                    continue;
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
            return map;
        }

        std::string _procNetDevPath;
        std::map<std::string, RawNet> _prev;
        bool _first = true;
    };

} // namespace

std::unique_ptr<NetworkPoller> CreateNetworkPoller(
    const std::string& procNetDevPath)
{
    return std::make_unique<NetworkPollerImpl>(procNetDevPath);
}

std::unique_ptr<NetworkPoller> CreateNetworkPoller(
    App& app, const std::string& procNetDevPath)
{
    auto poller = CreateNetworkPoller(procNetDevPath);
    poller->SetCacheIntervalFromApp(app);
    return poller;
}
