#pragma once

#include "sensors/disk_sensor.h"

#include <string>
#include <vector>

class MockDiskSensor : public DiskSensor
{
public:
    MockDiskSensor(std::vector<std::string> names = {},
        std::vector<std::vector<DiskSample>> samples = {}):
        DiskSensor("disk"),
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
    const DiskSample* GetSamples(std::size_t ch) const override
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
        return "Disk";
    }
    std::string GetPrefName() const override
    {
        return "disk";
    }
    void DrawConfiguration(Preferences& preferences) override
    {
        (void)preferences;
    }

    std::vector<std::string> _names;
    std::vector<std::vector<DiskSample>> _samples;
};
