#include "cpu_sensor.h"

#include <cctype>
#include <cstdint>
#include <fstream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

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

    class CpuSensorImpl : public CpuSensor
    {
    public:
        explicit CpuSensorImpl(const std::string& procStatPath):
            _procStatPath(procStatPath)
        {
            std::ifstream file(_procStatPath);
            std::string line;
            std::size_t count = 0;
            while (std::getline(file, line))
            {
                if (line.rfind("cpu", 0) != 0)
                    continue;
                if (line.size() > 3 &&
                    std::isdigit(static_cast<unsigned char>(line[3])))
                    count++;
            }
            _cpuCount = count;
            _samples.resize(_cpuCount);
            _prev.resize(_cpuCount);
            for (auto& v : _samples)
                v.reserve(_maxHistory);
        }

        std::size_t GetCpuCount() override
        {
            return _cpuCount;
        }

        std::size_t GetSampleCount() override
        {
            if (_samples.empty())
                return 0;
            return _samples[0].size();
        }

        const CpuSample* GetSamples(std::size_t cpu) override
        {
            if (cpu >= _samples.size())
                return nullptr;
            return _samples[cpu].data();
        }

        void Tick() override
        {
            std::ifstream file(_procStatPath);
            if (!file)
            {
                PushZeros();
                return;
            }

            std::string line;
            std::size_t index = 0;
            std::vector<RawTimes> cur(_cpuCount);

            while (std::getline(file, line))
            {
                if (line.rfind("cpu", 0) != 0)
                    continue;
                if (line.size() <= 3 ||
                    !std::isdigit(static_cast<unsigned char>(line[3])))
                    continue;
                if (index >= _cpuCount)
                    break;

                std::istringstream iss(line);
                std::string label;
                RawTimes t{};
                iss >> label >> t.user >> t.nice >> t.system >> t.idle >>
                    t.iowait >> t.irq >> t.softirq >> t.steal >> t.guest >>
                    t.guest_nice;
                cur[index] = t;
                index++;
            }

            if (index != _cpuCount)
            {
                PushZeros();
                return;
            }

            if (_first)
            {
                _prev = cur;
                _first = false;
                PushZeros();
                return;
            }

            for (std::size_t i = 0; i < _cpuCount; ++i)
            {
                const RawTimes& prev = _prev[i];
                const RawTimes& now = cur[i];

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

                CpuSample s{user, system, nice};
                auto& vec = _samples[i];
                if (vec.size() >= _maxHistory)
                    vec.erase(vec.begin());
                vec.push_back(s);
            }

            _prev = cur;
        }

    private:
        void PushZeros()
        {
            CpuSample zero{0.0f, 0.0f, 0.0f};
            for (auto& vec : _samples)
            {
                if (vec.size() >= _maxHistory)
                    vec.erase(vec.begin());
                vec.push_back(zero);
            }
        }

        static constexpr std::size_t _maxHistory = 120;
        std::string _procStatPath;
        std::size_t _cpuCount = 0;
        std::vector<std::vector<CpuSample>> _samples;
        std::vector<RawTimes> _prev;
        bool _first = true;
    };

} // namespace

std::unique_ptr<CpuSensor> CreateCpuSensor(const std::string& procStatPath)
{
    return std::make_unique<CpuSensorImpl>(procStatPath);
}
