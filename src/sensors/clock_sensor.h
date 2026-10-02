#pragma once

#include "app.h"

#include <ctime>

#include "sensor.h"

#include <memory>

class App;

// Fetches the current local time. Implementations live in
// src/sensors/clock_sensor_impl.cc.
class ClockSensor : public Sensor
{
public:
    explicit ClockSensor(App& app, const std::string& prefPrefix):
        Sensor(app, prefPrefix)
    {
    }

    explicit ClockSensor(const std::string& prefPrefix): Sensor(prefPrefix) {}

    virtual ~ClockSensor() = default;

    // Returns the current local time.
    virtual std::tm GetLocalTime() = 0;
};

extern std::unique_ptr<ClockSensor> CreateClockSensor(
    App& app, const std::string& prefPrefix);
