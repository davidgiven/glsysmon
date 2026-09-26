#include "memory_sensor.h"

#include <imgui.h>

#include <functional>
#include <map>
#include <memory>
#include <string>

#include "memory_poller.h"
#include "preferences/preferences.h"
#include "sensor_graph_mixin.h"
#include "timer.h"

namespace
{

    class MemorySensorImpl : public MemorySensor
    {
    public:
        explicit MemorySensorImpl(const Preferences& prefs,
            Timer& timer,
            const std::string& prefPrefix,
            std::shared_ptr<MemoryPoller> poller):
            MemorySensor(prefPrefix),
            _timer(timer),
            _poller(std::move(poller))
        {
            InitGraph(prefs, _prefPrefix, 1, MemorySample{0, 0});
            Tick(_timer.Now());
        }

        const MemorySample* GetSamples(std::size_t channel) const override
        {
            if (channel >= GetChannels())
                return nullptr;
            return this->SensorGraphMixin<MemorySample>::GetSamples(channel);
        }

        std::string GetChannelName(std::size_t channel) const override
        {
            if (channel == 0)
                return "mem";
            return std::to_string(channel);
        }

        std::string GetHumanName() const override
        {
            return "Memory";
        }

        std::string GetPrefName() const override
        {
            return "memory";
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
                        &MemorySensorImpl::Tick, this, std::placeholders::_1));
                return;
            }

            std::map<std::string, MemorySample> polled = _poller->Poll();
            if (polled.size() != channels)
            {
                PushZeros();
            }
            else
            {
                auto it = polled.find("mem");
                if (it == polled.end())
                {
                    AddSample(0, MemorySample{0, 0});
                }
                else
                {
                    AddSample(0, it->second);
                }
            }
            _timer.Schedule(t + this->_delta,
                std::bind(&MemorySensorImpl::Tick, this, std::placeholders::_1));
        }

        void PushZeros()
        {
            MemorySample zero{0, 0};
            for (std::size_t i = 0; i < GetChannels(); ++i)
                AddSample(i, zero);
        }

        Timer& _timer;
        std::shared_ptr<MemoryPoller> _poller;
    };

} // namespace

std::unique_ptr<MemorySensor> CreateMemorySensor(const Preferences& prefs,
    Timer& timer,
    const std::string& prefPrefix,
    std::shared_ptr<MemoryPoller> poller)
{
    return std::make_unique<MemorySensorImpl>(
        prefs, timer, prefPrefix, std::move(poller));
}

std::unique_ptr<MemorySensor> CreateMemorySensor(const Preferences& prefs,
    Timer& timer,
    const std::string& prefPrefix,
    const std::string& procMemInfoPath)
{
    auto poller = CreateMemoryPoller(procMemInfoPath);
    return CreateMemorySensor(prefs, timer, prefPrefix, std::move(poller));
}
