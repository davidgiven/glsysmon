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
