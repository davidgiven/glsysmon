#pragma once

#include "sensors/network_sensor.h"

#include <string>
#include <vector>

class MockNetworkSensor : public NetworkSensor
{
public:
    MockNetworkSensor(std::vector<std::string> names = {},
        std::vector<std::vector<NetworkSample>> samples = {}):
        NetworkSensor("network"),
        _names(std::move(names)),
        _samples(std::move(samples))
    {
    }

    std::size_t GetChannels() const override
    {
        return _samples.size();
    }

    std::size_t GetSampleCount() const override
    {
        if (_samples.empty())
            return 0;
        return _samples[0].size();
    }

    const NetworkSample* GetSamples(std::size_t ch) const override
    {
        if (ch >= _samples.size())
            return nullptr;
        return _samples[ch].data();
    }

    std::string GetChannelName(std::size_t channel) const override
    {
        if (channel < _names.size())
            return _names[channel];
        return "ch" + std::to_string(channel);
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
        (void)preferences;
    }

    std::vector<std::string> _names;
    std::vector<std::vector<NetworkSample>> _samples;
};
