#include "cpu_sensor.h"

#include <imgui.h>

#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstdint>
#include <fstream>
#include <functional>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "graph_mixin.h"
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
            std::size_t sampleCount = static_cast<std::size_t>(size);
            double interval =
                prefs.GetDouble("cpu.update_interval").value_or(5);
            if (interval <= 0)
                interval = 5;
            _delta = 1'000'000'000ULL / interval;

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
            InitGraph(count, sampleCount, CpuSample{});
            _prev.resize(count);
            Tick(_timer.Now());
        }

        const CpuSample* GetSamples(std::size_t cpu) const override
        {
            if (cpu >= GetChannels())
                return nullptr;
            return this->GraphMixin<CpuSample>::GetSamples(cpu);
        }

        std::string GetChannelName(std::size_t channel) const override
        {
            return "CPU" + std::to_string(channel);
        }

        std::string GetHumanName() const override
        {
            return "Cpu";
        }

        std::string GetPrefName() const override
        {
            return "cpu";
        }

        void DrawConfiguration(Preferences& preferences) override
        {
            (void)preferences;
            ImGui::Text("%s settings", GetHumanName().c_str());
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
                std::size_t channels = GetChannels();
                std::vector<RawTimes> cur(channels);

                while (std::getline(file, line))
                {
                    if (line.rfind("cpu", 0) != 0)
                        continue;
                    if (line.size() <= 3 ||
                        !std::isdigit(static_cast<unsigned char>(line[3])))
                        continue;
                    if (index >= channels)
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

                if (index != channels)
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
                        AddSample(i, s);
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
            for (std::size_t i = 0; i < GetChannels(); ++i)
                AddSample(i, zero);
        }

        Timer& _timer;
        std::string _procStatPath;
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
