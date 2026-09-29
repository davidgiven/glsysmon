#pragma once

#include <string>

class Context;
class Preferences;

// Fetches a piece of system data. Implementations live in sensor_*.cc.
class Sensor
{
public:
    explicit Sensor(const Context& ctx, const std::string& prefPrefix):
        _ctx(&ctx),
        _prefPrefix(prefPrefix)
    {
    }

    explicit Sensor(const std::string& prefPrefix):
        _ctx(nullptr),
        _prefPrefix(prefPrefix)
    {
    }

    virtual ~Sensor() = default;

    virtual std::string GetHumanName() const = 0;

    virtual std::string GetPrefName() const = 0;

    virtual void DrawConfiguration(Preferences& preferences);

protected:
    const Context* _ctx = nullptr;
    std::string _prefPrefix;
};
