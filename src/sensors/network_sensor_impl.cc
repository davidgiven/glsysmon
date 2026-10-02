#include "app.h"
#include "network_sensor.h"

#include <imgui.h>

#include <algorithm>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "network_poller.h"
#include "preferences/preferences.h"
#include "sensor_rx_tx_graph_mixin.h"
#include "timer.h"

namespace
{

    class NetworkSensorImpl : public NetworkSensor
    {
    public:
        explicit NetworkSensorImpl(App& app,
            const std::string& prefPrefix,
            std::shared_ptr<NetworkPoller> poller): NetworkSensor(app, prefPrefix),
            _app(app),
            _poller(std::move(poller))
        {
            auto initial = _poller->Poll();
            _ifaceNames.reserve(initial.size());
            for (const auto& kv : initial)
                _ifaceNames.push_back(kv.first);
            std::sort(_ifaceNames.begin(), _ifaceNames.end());
            InitGraph(_app,
                _prefPrefix,
                _ifaceNames.size(),
                RxTxSample{});
            Tick(_app.GetTimer().Now());
        }

        const RxTxSample* GetSamples(std::size_t channel) const override
        {
            if (channel >= GetChannels())
                return nullptr;
            return this->SensorRxTxGraphMixin::GetSamples(channel);
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
            DrawIntervalConfiguration(_app.GetImGui(), preferences, _prefPrefix);
        }

    private:
        void Tick(Timer::Time t)
        {
            std::size_t channels = GetChannels();
            if (channels == 0)
            {
                _app.GetTimer().Schedule(t + this->_delta,
                    std::bind(
                        &NetworkSensorImpl::Tick, this, std::placeholders::_1));
                return;
            }

            std::map<std::string, RxTxSample> polled = _poller->Poll();
            if (polled.size() != channels)
            {
                PushZeros();
            }
            else
            {
                for (std::size_t i = 0; i < channels; ++i)
                {
                    const std::string& name = _ifaceNames[i];
                    auto it = polled.find(name);
                    if (it == polled.end())
                    {
                        AddSample(i, RxTxSample{0, 0});
                        continue;
                    }
                    RxTxSample s = it->second;
                    s.rxBps *= this->_interval;
                    s.txBps *= this->_interval;
                    AddSample(i, s);
                }
            }
            _app.GetTimer().Schedule(t + this->_delta,
                std::bind(
                    &NetworkSensorImpl::Tick, this, std::placeholders::_1));
        }

        void PushZeros()
        {
            RxTxSample zero{0, 0};
            for (std::size_t i = 0; i < GetChannels(); ++i)
                AddSample(i, zero);
        }

        App& _app;
        std::shared_ptr<NetworkPoller> _poller;
        std::vector<std::string> _ifaceNames;
    };

} // namespace

std::unique_ptr<NetworkSensor> CreateNetworkSensor(App& app,
    const std::string& prefPrefix,
    std::shared_ptr<NetworkPoller> poller)
{
    return std::make_unique<NetworkSensorImpl>(
        app, prefPrefix, std::move(poller));
}

std::unique_ptr<NetworkSensor> CreateNetworkSensor(
    App& app,
    const std::string& prefPrefix,
    const std::string& procNetDevPath)
{
    auto poller = CreateNetworkPoller(procNetDevPath);
    return CreateNetworkSensor(app, prefPrefix, std::move(poller));
}
