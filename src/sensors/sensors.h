#pragma once

#include <memory>

#include "clock_sensor.h"
#include "hostname_sensor.h"

class Sensors
{
public:
    Sensors() = default;

    static Sensors& Instance();

    Sensors(const Sensors&) = delete;
    Sensors& operator=(const Sensors&) = delete;

    ClockSensor& GetClockSensor();
    HostnameSensor& GetHostnameSensor();

    void SetClockSensor(std::unique_ptr<ClockSensor> sensor);
    void SetHostnameSensor(std::unique_ptr<HostnameSensor> sensor);

    void Reset();

private:
    std::unique_ptr<ClockSensor> _clockSensor;
    std::unique_ptr<HostnameSensor> _hostnameSensor;
};
