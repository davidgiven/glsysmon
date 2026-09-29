#pragma once

#include "sensors/temperature_sensor.h"

#include <string>
#include <vector>

class MockTemperatureSensor : public TemperatureSensor
{
public:
    MockTemperatureSensor(std::vector<std::string> names = {},
        std::vector<std::vector<double>> samples = {}):
        TemperatureSensor("temperature"),
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

    const double* GetSamples(std::size_t ch) const override
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
        return "Temperature";
    }

    std::string GetPrefName() const override
    {
        return "temperature";
    }

    void DrawConfiguration(Preferences& preferences) override
    {
        (void)preferences;
    }

    std::vector<std::string> _names;
    std::vector<std::vector<double>> _samples;
};
