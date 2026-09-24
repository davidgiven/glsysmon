#include "sensors.h"

#include <optional>
#include <string>
#include <vector>

#include "preferences/preferences.h"
#include "sensors/clock_sensor.h"
#include "sensors/cpu_sensor.h"
#include "sensors/hostname_sensor.h"
#include "sensors/temperature_sensor.h"

Sensors::Sensors(const Preferences& prefs, Timer& timer):
    _prefs(prefs),
    _timer(timer)
{
}

ClockSensor& Sensors::GetClockSensor() const
{
    if (_clockSensor == nullptr)
        _clockSensor = CreateClockSensor(_prefs, _timer);
    return *_clockSensor;
}

CpuSensor& Sensors::GetCpuSensor() const
{
    if (_cpuSensor == nullptr)
        _cpuSensor = CreateCpuSensor(_prefs, _timer);
    return *_cpuSensor;
}

HostnameSensor& Sensors::GetHostnameSensor() const
{
    if (_hostnameSensor == nullptr)
        _hostnameSensor = CreateHostnameSensor(_prefs, _timer);
    return *_hostnameSensor;
}

TemperatureSensor& Sensors::GetTemperatureSensor() const
{
    if (_temperatureSensor == nullptr)
        _temperatureSensor = CreateTemperatureSensor(_prefs, _timer);
    return *_temperatureSensor;
}

std::vector<Sensor*> Sensors::GetAllSensors() const
{
    std::vector<Sensor*> sensors;
    sensors.reserve(4);
    sensors.push_back(&GetClockSensor());
    sensors.push_back(&GetCpuSensor());
    sensors.push_back(&GetHostnameSensor());
    sensors.push_back(&GetTemperatureSensor());
    return sensors;
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

void Sensors::SetTemperatureSensor(std::unique_ptr<TemperatureSensor> sensor)
{
    _temperatureSensor = std::move(sensor);
}

void Sensors::Reset()
{
    _clockSensor.reset();
    _cpuSensor.reset();
    _hostnameSensor.reset();
    _temperatureSensor.reset();
}
