#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <span>
#include <string>
#include <vector>

#include "preferences/preferences.h"

template <typename T>
class SensorGraphMixin
{
public:
    SensorGraphMixin() = default;

    explicit SensorGraphMixin(
        std::size_t channels, std::size_t sampleCount, const T& initial = T{}):
        _samples(channels, std::vector<T>(sampleCount, initial))
    {
    }

    virtual ~SensorGraphMixin() = default;

    virtual std::size_t GetChannels()
    {
        return const_cast<const SensorGraphMixin*>(this)->GetChannels();
    }

    virtual std::size_t GetChannels() const
    {
        return _samples.size();
    }

    virtual std::size_t GetSampleCount()
    {
        return const_cast<const SensorGraphMixin*>(this)->GetSampleCount();
    }

    virtual std::size_t GetSampleCount() const
    {
        if (_samples.empty())
            return 0;
        return _samples[0].size();
    }

    virtual const T* GetSamples(std::size_t channel)
    {
        return const_cast<const SensorGraphMixin*>(this)->GetSamples(channel);
    }

    virtual const T* GetSamples(std::size_t channel) const
    {
        assert(channel < _samples.size());
        if (channel >= _samples.size())
            return nullptr;
        return _samples[channel].data();
    }

    virtual std::string GetChannelName(std::size_t channel) const = 0;

    std::span<const T> GetSamplesSpan(std::size_t channel)
    {
        const T* data = GetSamples(channel);
        if (data == nullptr)
            return {};
        return {data, GetSampleCount()};
    }

    std::span<const T> GetSamplesSpan(std::size_t channel) const
    {
        const T* data = GetSamples(channel);
        if (data == nullptr)
            return {};
        return {data, GetSampleCount()};
    }

protected:
    void AddSample(std::size_t channel, T sample)
    {
        assert(channel < _samples.size());
        if (channel >= _samples.size())
            return;
        auto& vec = _samples[channel];
        if (vec.empty())
            return;
        if (vec.size() > 1)
            std::copy(vec.begin() + 1, vec.end(), vec.begin());
        vec.back() = sample;
    }

    void InitGraph(
        std::size_t channels, std::size_t sampleCount, const T& initial = T{})
    {
        _samples.assign(channels, std::vector<T>(sampleCount, initial));
    }

    void InitGraph(const Preferences& prefs,
        const std::string& prefPrefix,
        std::size_t channels,
        const T& initial = T{},
        double defaultInterval = 1)
    {
        int size = GlobalPreferencesFetcher::GetSize(prefs);
        if (size <= 0)
            size = 1;
        std::size_t sampleCount = static_cast<std::size_t>(size);
        double interval = prefs.GetDouble(prefPrefix + ".update_interval")
                              .value_or(defaultInterval);
        if (interval <= 0)
            interval = defaultInterval;
        _interval = interval;
        _delta = static_cast<std::uint64_t>(1'000'000'000ULL / interval);
        _samples.assign(channels, std::vector<T>(sampleCount, initial));
    }

    std::uint64_t _delta = 0;
    double _interval = 1;

private:
    std::vector<std::vector<T>> _samples;
};
