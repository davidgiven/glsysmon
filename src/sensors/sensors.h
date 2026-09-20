#pragma once

#include <memory>

#include "clock_sensor.h"
#include "cpu_sensor.h"
#include "hostname_sensor.h"

class Preferences;

class Sensors
{
public:
    explicit Sensors(const Preferences& prefs);

    Sensors(const Sensors&) = delete;
    Sensors& operator=(const Sensors&) = delete;

    ClockSensor& GetClockSensor();
    CpuSensor& GetCpuSensor();
    HostnameSensor& GetHostnameSensor();

    void SetClockSensor(std::unique_ptr<ClockSensor> sensor);
    void SetCpuSensor(std::unique_ptr<CpuSensor> sensor);
    void SetHostnameSensor(std::unique_ptr<HostnameSensor> sensor);

    void Reset();

private:
    const Preferences& _prefs;
    std::unique_ptr<ClockSensor> _clockSensor;
    std::unique_ptr<CpuSensor> _cpuSensor;
    std::unique_ptr<HostnameSensor> _hostnameSensor;
};
