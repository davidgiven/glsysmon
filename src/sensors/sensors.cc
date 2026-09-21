#include "sensors.h"

#include <optional>
#include <string>
#include <vector>

#include "components.h"
#include "preferences/preferences.h"

Sensors::Sensors(const Preferences& prefs, Timer* timer):
    _prefs(prefs),
    _timer(timer)
{
}

ClockSensor& Sensors::GetClockSensor()
{
    if (_clockSensor == nullptr)
        _clockSensor = CreateClockSensor(_prefs, _timer);
    return *_clockSensor;
}

CpuSensor& Sensors::GetCpuSensor()
{
    if (_cpuSensor == nullptr)
        _cpuSensor = CreateCpuSensor(_prefs, _timer);
    return *_cpuSensor;
}

HostnameSensor& Sensors::GetHostnameSensor()
{
    if (_hostnameSensor == nullptr)
        _hostnameSensor = CreateHostnameSensor(_prefs, _timer);
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

void Sensors::Tick()
{
    if (_clockSensor != nullptr)
        _clockSensor->Tick();
    if (_cpuSensor != nullptr)
        _cpuSensor->Tick();
    if (_hostnameSensor != nullptr)
        _hostnameSensor->Tick();
}

void Sensors::Reset()
{
    _clockSensor.reset();
    _cpuSensor.reset();
    _hostnameSensor.reset();
}
