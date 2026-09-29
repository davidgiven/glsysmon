#include "memory_sensor.h"

#include <imgui.h>

#include <functional>
#include <map>
#include <memory>
#include <string>

#include "memory_poller.h"
#include "context.h"
#include "preferences/preferences.h"
#include "sensor_graph_mixin.h"
#include "timer.h"

namespace
{

    class MemorySensorImpl : public MemorySensor
    {
    public:
        explicit MemorySensorImpl(const Context& ctx,
            const std::string& prefPrefix,
            std::shared_ptr<MemoryPoller> poller): MemorySensor(ctx, prefPrefix),
            _ctx(ctx),
            _poller(std::move(poller))
        {
            InitGraph(_ctx, _prefPrefix, 1, MemorySample{0, 0});
            Tick(_ctx.timer.Now());
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
            DrawIntervalConfiguration(_ctx.imgui, preferences, _prefPrefix);
        }

    private:
        void Tick(Timer::Time t)
        {
            std::size_t channels = GetChannels();
            if (channels == 0)
            {
                _ctx.timer.Schedule(t + this->_delta,
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
            _ctx.timer.Schedule(t + this->_delta,
                std::bind(
                    &MemorySensorImpl::Tick, this, std::placeholders::_1));
        }

        void PushZeros()
        {
            MemorySample zero{0, 0};
            for (std::size_t i = 0; i < GetChannels(); ++i)
                AddSample(i, zero);
        }

        const Context& _ctx;
        std::shared_ptr<MemoryPoller> _poller;
    };

} // namespace

std::unique_ptr<MemorySensor> CreateMemorySensor(const Context& ctx,
    const std::string& prefPrefix,
    std::shared_ptr<MemoryPoller> poller)
{
    return std::make_unique<MemorySensorImpl>(
        ctx, prefPrefix, std::move(poller));
}
