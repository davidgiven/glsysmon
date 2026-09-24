#pragma once

#include <string>

#include "sensor.h"

#include <memory>

class Preferences;
class Timer;

// Fetches the current system hostname. Implementations live in
// src/sensors/hostname_sensor_impl.cc.
class HostnameSensor : public Sensor
{
public:
    virtual ~HostnameSensor() = default;

    // Returns the current hostname, or an empty string if it cannot be read.
    virtual std::string GetHostname() = 0;
};

extern std::unique_ptr<HostnameSensor> CreateHostnameSensor(
    const Preferences& prefs, Timer& timer);
