#include "context.h"
#include "cpu_poller.h"

#include <cctype>
#include <cstdint>
#include <fstream>
#include <map>
#include <memory>
#include <sstream>
#include <string>

namespace
{

    struct RawTimes
    {
        std::uint64_t user = 0;
        std::uint64_t nice = 0;
        std::uint64_t system = 0;
        std::uint64_t idle = 0;
        std::uint64_t iowait = 0;
        std::uint64_t irq = 0;
        std::uint64_t softirq = 0;
        std::uint64_t steal = 0;
        std::uint64_t guest = 0;
        std::uint64_t guest_nice = 0;
    };

    class CpuPollerImpl : public CpuPoller
    {
    public:
        explicit CpuPollerImpl(const std::string& procStatPath):
            _procStatPath(procStatPath)
        {
        }

        std::map<std::string, CpuSample> Poll() override
        {
            std::map<std::string, RawTimes> cur;
            std::ifstream file(_procStatPath);
            if (!file)
                return {};

            std::string line;
            while (std::getline(file, line))
            {
                if (line.rfind("cpu", 0) != 0)
                    continue;
                if (line.size() <= 3 ||
                    !std::isdigit(static_cast<unsigned char>(line[3])))
                    continue;

                std::istringstream iss(line);
                std::string label;
                RawTimes t{};
                iss >> label >> t.user >> t.nice >> t.system >> t.idle >>
                    t.iowait >> t.irq >> t.softirq >> t.steal >> t.guest >>
                    t.guest_nice;
                if (!iss)
                    continue;
                std::string key = label.substr(3);
                cur[key] = t;
            }

            std::map<std::string, CpuSample> result;
            if (_first)
            {
                _first = false;
                _prev = cur;
                for (const auto& kv : cur)
                    result[kv.first] = CpuSample{0.0f, 0.0f, 0.0f};
                return result;
            }

            for (const auto& kv : cur)
            {
                const std::string& key = kv.first;
                const RawTimes& now = kv.second;
                auto it = _prev.find(key);
                if (it == _prev.end())
                {
                    result[key] = CpuSample{0.0f, 0.0f, 0.0f};
                    continue;
                }
                const RawTimes& prev = it->second;

                std::uint64_t prevIdle = prev.idle + prev.iowait;
                std::uint64_t nowIdle = now.idle + now.iowait;

                std::uint64_t prevNonIdle =
                    prev.user + prev.nice + prev.system + prev.irq +
                    prev.softirq + prev.steal + prev.guest + prev.guest_nice;
                std::uint64_t nowNonIdle = now.user + now.nice + now.system +
                                           now.irq + now.softirq + now.steal +
                                           now.guest + now.guest_nice;

                std::uint64_t prevTotal = prevIdle + prevNonIdle;
                std::uint64_t nowTotal = nowIdle + nowNonIdle;

                std::uint64_t totalDelta = nowTotal - prevTotal;
                float user = 0.0f;
                float system = 0.0f;
                float nice = 0.0f;
                if (totalDelta != 0)
                {
                    user = static_cast<float>(now.user - prev.user) /
                           static_cast<float>(totalDelta);
                    system = static_cast<float>(now.system - prev.system) /
                             static_cast<float>(totalDelta);
                    nice = static_cast<float>(now.nice - prev.nice) /
                           static_cast<float>(totalDelta);
                }

                result[key] = CpuSample{user, system, nice};
            }

            _prev = cur;
            return result;
        }

    private:
        std::string _procStatPath;
        std::map<std::string, RawTimes> _prev;
        bool _first = true;
    };

} // namespace

std::unique_ptr<CpuPoller> CreateCpuPoller(const std::string& procStatPath)
{
    return std::make_unique<CpuPollerImpl>(procStatPath);
}

std::unique_ptr<CpuPoller> CreateCpuPoller(
    const Context& ctx, const std::string& procStatPath)
{
    auto poller = CreateCpuPoller(procStatPath);
    poller->SetCacheIntervalFromContext(ctx);
    return poller;
}
