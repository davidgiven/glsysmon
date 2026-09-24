#pragma once

#include <string>

class Preferences;

// Fetches a piece of system data. Implementations live in sensor_*.cc.
class Sensor
{
public:
    explicit Sensor(const std::string& prefPrefix): _prefPrefix(prefPrefix)
    {
    }

    virtual ~Sensor() = default;

    virtual std::string GetHumanName() const = 0;

    virtual std::string GetPrefName() const = 0;

    virtual void DrawConfiguration(Preferences& preferences);

protected:
    std::string _prefPrefix;
};
