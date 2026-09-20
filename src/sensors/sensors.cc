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

CpuSensor& Sensors::GetCpuSensor()
{
    if (_cpuSensor == nullptr)
        _cpuSensor = CreateCpuSensor();
    return *_cpuSensor;
}

HostnameSensor& Sensors::GetHostnameSensor()
{
    if (_hostnameSensor == nullptr)
        _hostnameSensor = CreateHostnameSensor();
    return *_hostnameSensor;
}

void Sensors::SetClockSensor(std::unique_ptr<ClockSensor> sensor)
{
    _clockSensor = std::move(sensor);
}

void Sensors::SetCpuSensor(std::unique_ptr<CpuSensor> sensor)
{
    _cpuSensor = std::move(sensor);
}

void Sensors::SetHostnameSensor(std::unique_ptr<HostnameSensor> sensor)
{
    _hostnameSensor = std::move(sensor);
}

void Sensors::Reset()
{
    _clockSensor.reset();
    _cpuSensor.reset();
    _hostnameSensor.reset();
}
