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

std::unique_ptr<ClockSensor> Sensors::CreateClockSensor(
    const std::string& prefPrefix) const
{
    return ::CreateClockSensor(_prefs, _timer, prefPrefix);
}

std::unique_ptr<CpuSensor> Sensors::CreateCpuSensor(
    const std::string& a, const std::string& b) const
{
    return ::CreateCpuSensor(_prefs, _timer, a, b);
}

std::unique_ptr<HostnameSensor> Sensors::CreateHostnameSensor(
    const std::string& prefPrefix) const
{
    return ::CreateHostnameSensor(_prefs, _timer, prefPrefix);
}

std::unique_ptr<TemperatureSensor> Sensors::CreateTemperatureSensor(
    const std::string& a, const std::string& b) const
{
    return ::CreateTemperatureSensor(_prefs, _timer, a, b);
}
