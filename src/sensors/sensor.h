#pragma once

#include <string>

// Fetches a piece of system data. Implementations live in sensor_*.cc.
class Sensor
{
public:
    virtual ~Sensor() = default;

    virtual std::string GetName() const = 0;
};
