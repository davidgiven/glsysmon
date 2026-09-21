#pragma once

#include <string>

#include "sensor.h"

// Fetches the current system hostname. Implementations live in
// src/sensors/hostname_sensor_impl.cc.
class HostnameSensor : public Sensor
{
public:
    virtual ~HostnameSensor() = default;

    // Returns the current hostname, or an empty string if it cannot be read.
    virtual std::string GetHostname() = 0;
};
