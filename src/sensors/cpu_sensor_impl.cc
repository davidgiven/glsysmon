#include "cpu_sensor.h"

#include <imgui.h>

#include <algorithm>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "cpu_poller.h"
#include "preferences/preferences.h"
#include "sensor_graph_mixin.h"
#include "timer.h"

namespace
{

    class CpuSensorImpl : public CpuSensor
    {
    public:
        explicit CpuSensorImpl(const Preferences& prefs,
            Timer& timer,
            const std::string& prefPrefix,
            std::shared_ptr<CpuPoller> poller):
            CpuSensor(prefPrefix),
            _timer(timer),
            _poller(std::move(poller))
        {
            auto initial = _poller->Poll();
            _cpuNames.reserve(initial.size());
            for (const auto& kv : initial)
                _cpuNames.push_back(kv.first);
            std::sort(_cpuNames.begin(),
                _cpuNames.end(),
                [](const std::string& a, const std::string& b)
                {
                    try
                    {
                        return std::stoi(a) < std::stoi(b);
                    }
                    catch (...)
                    {
                        return a < b;
                    }
                });
            InitGraph(prefs, _prefPrefix, _cpuNames.size(), CpuSample{}, 5);
            Tick(_timer.Now());
        }

        const CpuSample* GetSamples(std::size_t cpu) const override
        {
            if (cpu >= GetChannels())
                return nullptr;
            return this->SensorGraphMixin<CpuSample>::GetSamples(cpu);
        }

        std::string GetChannelName(std::size_t channel) const override
        {
            if (channel < _cpuNames.size())
                return _cpuNames[channel];
            return std::to_string(channel);
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
            DrawIntervalConfiguration(preferences, _prefPrefix);
        }

    private:
        void Tick(Timer::Time t)
        {
            std::size_t channels = GetChannels();
            if (channels == 0)
            {
                _timer.Schedule(t + this->_delta,
                    std::bind(
                        &CpuSensorImpl::Tick, this, std::placeholders::_1));
                return;
            }

            std::map<std::string, CpuSample> polled = _poller->Poll();
            if (polled.size() != channels)
            {
                PushZeros();
            }
            else
            {
                for (std::size_t i = 0; i < channels; ++i)
                {
                    const std::string& name = _cpuNames[i];
                    auto it = polled.find(name);
                    if (it == polled.end())
                    {
                        AddSample(i, CpuSample{0.0f, 0.0f, 0.0f});
                        continue;
                    }
                    AddSample(i, it->second);
                }
            }
            _timer.Schedule(t + this->_delta,
                std::bind(&CpuSensorImpl::Tick, this, std::placeholders::_1));
        }

        void PushZeros()
        {
            CpuSample zero{0.0f, 0.0f, 0.0f};
            for (std::size_t i = 0; i < GetChannels(); ++i)
                AddSample(i, zero);
        }

        Timer& _timer;
        std::shared_ptr<CpuPoller> _poller;
        std::vector<std::string> _cpuNames;
    };

} // namespace

std::unique_ptr<CpuSensor> CreateCpuSensor(const Preferences& prefs,
    Timer& timer,
    const std::string& prefPrefix,
    std::shared_ptr<CpuPoller> poller)
{
    return std::make_unique<CpuSensorImpl>(
        prefs, timer, prefPrefix, std::move(poller));
}
