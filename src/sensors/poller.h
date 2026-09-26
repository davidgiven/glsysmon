#pragma once

#include <chrono>
#include <map>
#include <optional>
#include <string>

// Fetches a piece of system data. Implementations live in poller_*.cc.
template <typename T>
class Poller
{
public:
    virtual ~Poller() = default;

    virtual std::map<std::string, T> Poll() = 0;

    std::map<std::string, T> PollCached();

    void SetCacheInterval(std::chrono::milliseconds interval)
    {
        _cacheInterval = interval;
    }

    void SetCacheIntervalMs(int ms)
    {
        _cacheInterval = std::chrono::milliseconds(ms);
    }

    std::chrono::milliseconds GetCacheInterval() const
    {
        return _cacheInterval;
    }

    int GetCacheIntervalMs() const
    {
        return static_cast<int>(_cacheInterval.count());
    }

protected:
    virtual std::chrono::steady_clock::time_point Now() const
    {
        return std::chrono::steady_clock::now();
    }

private:
    std::map<std::string, T> _cached{};
    std::optional<std::chrono::steady_clock::time_point> _lastPoll{};
    std::chrono::milliseconds _cacheInterval{500};
};

template <typename T>
std::map<std::string, T> Poller<T>::PollCached()
{
    auto now = Now();
    if (_lastPoll.has_value())
    {
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
            now - *_lastPoll);
        if (elapsed < _cacheInterval)
            return _cached;
    }
    _cached = Poll();
    _lastPoll = now;
    return _cached;
}
