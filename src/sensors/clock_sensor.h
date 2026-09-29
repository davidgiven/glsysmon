#pragma once

#include <ctime>

#include "sensor.h"

#include <memory>

class Context;

// Fetches the current local time. Implementations live in
// src/sensors/clock_sensor_impl.cc.
class ClockSensor : public Sensor
{
public:
    explicit ClockSensor(const Context& ctx, const std::string& prefPrefix):
        Sensor(ctx, prefPrefix)
    {
    }

    explicit ClockSensor(const std::string& prefPrefix): Sensor(prefPrefix) {}

    virtual ~ClockSensor() = default;

    // Returns the current local time.
    virtual std::tm GetLocalTime() = 0;
};

extern std::unique_ptr<ClockSensor> CreateClockSensor(
    const Context& ctx, const std::string& prefPrefix);
