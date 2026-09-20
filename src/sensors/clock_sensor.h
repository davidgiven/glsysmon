#pragma once

#include <ctime>

// Fetches the current local time. Implementations live in
// src/sensors/clock_sensor_impl.cc.
class ClockSensor
{
public:
    virtual ~ClockSensor() = default;

    // Returns the current local time.
    virtual std::tm GetLocalTime() = 0;
};
