#pragma once

#include <memory>
#include <string>
#include <vector>

#include "clock_sensor.h"
#include "cpu_sensor.h"
#include "hostname_sensor.h"
#include "temperature_sensor.h"

class Preferences;
class Sensor;
class Timer;

class Sensors
{
public:
    explicit Sensors(const Preferences& prefs, Timer& timer);

    Sensors(const Sensors&) = delete;
    Sensors& operator=(const Sensors&) = delete;

    std::unique_ptr<ClockSensor> CreateClockSensor() const;
    std::unique_ptr<CpuSensor> CreateCpuSensor() const;
    std::unique_ptr<CpuSensor> CreateCpuSensor(
        const std::string& procStatPath) const;
    std::unique_ptr<HostnameSensor> CreateHostnameSensor() const;
    std::unique_ptr<TemperatureSensor> CreateTemperatureSensor() const;
    std::unique_ptr<TemperatureSensor> CreateTemperatureSensor(
        const std::string& hwmonRoot) const;

    // Legacy registry API retained for compatibility with older tests.
    ClockSensor& GetClockSensor() const;
    CpuSensor& GetCpuSensor() const;
    HostnameSensor& GetHostnameSensor() const;
    TemperatureSensor& GetTemperatureSensor() const;

    void SetClockSensor(std::unique_ptr<ClockSensor> sensor);
    void SetCpuSensor(std::unique_ptr<CpuSensor> sensor);
    void SetHostnameSensor(std::unique_ptr<HostnameSensor> sensor);
    void SetTemperatureSensor(std::unique_ptr<TemperatureSensor> sensor);

    void Reset();

private:
    const Preferences& _prefs;
    Timer& _timer;
    mutable std::unique_ptr<ClockSensor> _clockSensor;
    mutable std::unique_ptr<CpuSensor> _cpuSensor;
    mutable std::unique_ptr<HostnameSensor> _hostnameSensor;
    mutable std::unique_ptr<TemperatureSensor> _temperatureSensor;
};
