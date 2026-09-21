#pragma once

#include <memory>

#include "clock_sensor.h"
#include "cpu_sensor.h"
#include "hostname_sensor.h"

class Preferences;
class Timer;

class Sensors
{
public:
    explicit Sensors(const Preferences& prefs, Timer* timer = nullptr);

    Sensors(const Sensors&) = delete;
    Sensors& operator=(const Sensors&) = delete;

    ClockSensor& GetClockSensor();
    CpuSensor& GetCpuSensor();
    HostnameSensor& GetHostnameSensor();

    void SetClockSensor(std::unique_ptr<ClockSensor> sensor);
    void SetCpuSensor(std::unique_ptr<CpuSensor> sensor);
    void SetHostnameSensor(std::unique_ptr<HostnameSensor> sensor);

    void Tick();

    void Reset();

private:
    const Preferences& _prefs;
    Timer* _timer;
    std::unique_ptr<ClockSensor> _clockSensor;
    std::unique_ptr<CpuSensor> _cpuSensor;
    std::unique_ptr<HostnameSensor> _hostnameSensor;
};
