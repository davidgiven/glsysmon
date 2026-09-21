#pragma once

#include <cstdint>
#include <functional>
#include <memory>

class Timer
{
public:
    using Time = uint64_t;
    using Callback = std::function<void(Time)>;

    virtual ~Timer() = default;

    virtual Time Schedule(Time time, Callback callback) = 0;
    virtual void Cancel(Time time) = 0;
    virtual void Tick(Time now) = 0;
};

extern std::unique_ptr<Timer> CreateTimer();
