#include "sensors.h"

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

std::unique_ptr<ClockSensor> Sensors::CreateClockSensor() const
{
    return ::CreateClockSensor(_prefs, _timer);
}

std::unique_ptr<CpuSensor> Sensors::CreateCpuSensor() const
{
    return ::CreateCpuSensor(_prefs, _timer);
}

std::unique_ptr<CpuSensor> Sensors::CreateCpuSensor(
    const std::string& procStatPath) const
{
    return ::CreateCpuSensor(_prefs, _timer, procStatPath);
}

std::unique_ptr<HostnameSensor> Sensors::CreateHostnameSensor() const
{
    return ::CreateHostnameSensor(_prefs, _timer);
}

std::unique_ptr<TemperatureSensor> Sensors::CreateTemperatureSensor() const
{
    return ::CreateTemperatureSensor(_prefs, _timer);
}

std::unique_ptr<TemperatureSensor> Sensors::CreateTemperatureSensor(
    const std::string& hwmonRoot) const
{
    return ::CreateTemperatureSensor(_prefs, _timer, hwmonRoot);
}

ClockSensor& Sensors::GetClockSensor() const
{
    if (_clockSensor == nullptr)
        _clockSensor = CreateClockSensor();
    return *_clockSensor;
}

CpuSensor& Sensors::GetCpuSensor() const
{
    if (_cpuSensor == nullptr)
        _cpuSensor = CreateCpuSensor();
    return *_cpuSensor;
}

HostnameSensor& Sensors::GetHostnameSensor() const
{
    if (_hostnameSensor == nullptr)
        _hostnameSensor = CreateHostnameSensor();
    return *_hostnameSensor;
}

TemperatureSensor& Sensors::GetTemperatureSensor() const
{
    if (_temperatureSensor == nullptr)
        _temperatureSensor = CreateTemperatureSensor();
    return *_temperatureSensor;
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
