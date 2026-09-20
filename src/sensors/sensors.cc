#include "sensors.h"

#include "components.h"

Sensors& Sensors::Instance()
{
    static Sensors instance;
    return instance;
}

ClockSensor& Sensors::GetClockSensor()
{
    if (_clockSensor == nullptr)
        _clockSensor = CreateClockSensor();
    return *_clockSensor;
}

HostnameSensor& Sensors::GetHostnameSensor()
{
    if (_hostnameSensor == nullptr)
        _hostnameSensor = CreateHostnameSensor();
    return *_hostnameSensor;
}

void Sensors::Reset()
{
    _clockSensor.reset();
    _hostnameSensor.reset();
}
