#pragma once

// Fetches a piece of system data. Implementations live in sensor_*.cc.
class Sensor
{
public:
    virtual ~Sensor() = default;

    // Updates the sensor's cached data.
    virtual void Tick() = 0;
};
