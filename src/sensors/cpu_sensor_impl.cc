#include "cpu_sensor.h"

#include <algorithm>
#include <cctype>
#include <cstdint>
#include <fstream>
#include <functional>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "preferences/preferences.h"
#include "timer.h"

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
        explicit CpuSensorImpl(const Preferences& prefs,
            Timer& timer,
            const std::string& procStatPath):
            _timer(timer),
            _procStatPath(procStatPath)
        {
            int size = GlobalPreferencesFetcher::GetSize(prefs);
            if (size <= 0)
                size = 1;
            _sampleCount = static_cast<std::size_t>(size);
            int fps = GlobalPreferencesFetcher::GetFps(prefs);
            if (fps <= 0)
                fps = 10;
            _delta = 1'000'000'000ULL / static_cast<uint64_t>(fps);

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
            _samples.assign(_cpuCount,
                std::vector<CpuSample>(
                    _sampleCount, CpuSample{0.0f, 0.0f, 0.0f}));
            _prev.resize(_cpuCount);
            Tick(_timer.Now());
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

    private:
        void Tick(Timer::Time t)
        {
            std::ifstream file(_procStatPath);
            if (!file)
            {
                PushZeros();
            }
            else
            {
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
                    RawTimes tRaw{};
                    iss >> label >> tRaw.user >> tRaw.nice >> tRaw.system >>
                        tRaw.idle >> tRaw.iowait >> tRaw.irq >> tRaw.softirq >>
                        tRaw.steal >> tRaw.guest >> tRaw.guest_nice;
                    cur[index] = tRaw;
                    index++;
                }

                if (index != _cpuCount)
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
                    for (std::size_t i = 0; i < _cpuCount; ++i)
                    {
                        const RawTimes& prev = _prev[i];
                        const RawTimes& now = cur[i];

                        std::uint64_t prevIdle = prev.idle + prev.iowait;
                        std::uint64_t nowIdle = now.idle + now.iowait;

                        std::uint64_t prevNonIdle =
                            prev.user + prev.nice + prev.system + prev.irq +
                            prev.softirq + prev.steal + prev.guest +
                            prev.guest_nice;
                        std::uint64_t nowNonIdle = now.user + now.nice +
                                                   now.system + now.irq +
                                                   now.softirq + now.steal +
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
                            system =
                                static_cast<float>(now.system - prev.system) /
                                static_cast<float>(totalDelta);
                            nice = static_cast<float>(now.nice - prev.nice) /
                                   static_cast<float>(totalDelta);
                        }

                        CpuSample s{user, system, nice};
                        auto& vec = _samples[i];
                        if (!vec.empty())
                        {
                            if (vec.size() > 1)
                                std::copy(
                                    vec.begin() + 1, vec.end(), vec.begin());
                            vec.back() = s;
                        }
                    }

                    _prev = cur;
                }
            }
            _timer.Schedule(t + _delta,
                std::bind(&CpuSensorImpl::Tick, this, std::placeholders::_1));
        }

        void PushZeros()
        {
            CpuSample zero{0.0f, 0.0f, 0.0f};
            for (auto& vec : _samples)
            {
                if (vec.empty())
                    continue;
                if (vec.size() > 1)
                    std::copy(vec.begin() + 1, vec.end(), vec.begin());
                vec.back() = zero;
            }
        }

        Timer& _timer;
        std::string _procStatPath;
        std::size_t _sampleCount = 0;
        std::size_t _cpuCount = 0;
        std::vector<std::vector<CpuSample>> _samples;
        std::vector<RawTimes> _prev;
        bool _first = true;
        uint64_t _delta = 0;
    };

} // namespace

std::unique_ptr<CpuSensor> CreateCpuSensor(
    const Preferences& prefs, Timer& timer, const std::string& procStatPath)
{
    return std::make_unique<CpuSensorImpl>(prefs, timer, procStatPath);
}

std::unique_ptr<CpuSensor> CreateCpuSensor(
    const Preferences& prefs, Timer& timer)
{
    return CreateCpuSensor(prefs, timer, "/proc/stat");
}
