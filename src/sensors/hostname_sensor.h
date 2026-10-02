#pragma once

#include "app.h"

#include <string>

#include "sensor.h"

#include <memory>

class App;

// Fetches the current system hostname. Implementations live in
// src/sensors/hostname_sensor_impl.cc.
class HostnameSensor : public Sensor
{
public:
    explicit HostnameSensor(App& app, const std::string& prefPrefix):
        Sensor(app, prefPrefix)
    {
    }

    explicit HostnameSensor(const std::string& prefPrefix): Sensor(prefPrefix) {}

    virtual ~HostnameSensor() = default;

    // Returns the current hostname, or an empty string if it cannot be read.
    virtual std::string GetHostname() = 0;
};

extern std::unique_ptr<HostnameSensor> CreateHostnameSensor(
    App& app, const std::string& prefPrefix);
