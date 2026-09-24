#pragma once

#include <string>

class Preferences;

// Fetches a piece of system data. Implementations live in sensor_*.cc.
class Sensor
{
public:
    virtual ~Sensor() = default;

    virtual std::string GetHumanName() const = 0;

    virtual std::string GetPrefName() const = 0;

    virtual void DrawConfiguration(Preferences& preferences) = 0;
};
