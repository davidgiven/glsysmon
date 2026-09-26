#pragma once

#include <memory>

class Preferences;
class Timer;
class App;
class HostnameSensor;
class ClockSensor;
class CpuSensor;
class DiskSensor;
class NetworkSensor;
class TemperatureSensor;

struct SDL_Window;

// UI backend interface. Implementations live in ui.cpp.
class Ui
{
public:
    virtual ~Ui() = default;

    virtual void Draw() = 0;
};

extern std::unique_ptr<Ui> CreateUi(
    const Preferences& prefs, Timer& timer, App& app);
extern std::unique_ptr<Ui> CreateUiWithFakeHostname(const Preferences& prefs,
    Timer& timer,
    std::unique_ptr<HostnameSensor> fakeSensor,
    App& app);
extern std::unique_ptr<Ui> CreateUiWithFakeClock(const Preferences& prefs,
    Timer& timer,
    std::unique_ptr<ClockSensor> fakeSensor,
    App& app);
extern std::unique_ptr<Ui> CreateUiWithFakeCpu(const Preferences& prefs,
    Timer& timer,
    std::unique_ptr<CpuSensor> fakeSensor,
    App& app);
extern std::unique_ptr<Ui> CreateUiWithFakeTemperature(const Preferences& prefs,
    Timer& timer,
    std::unique_ptr<TemperatureSensor> fakeSensor,
    App& app);
extern std::unique_ptr<Ui> CreateUiWithFakeNetwork(const Preferences& prefs,
    Timer& timer,
    std::unique_ptr<NetworkSensor> fakeSensor,
    App& app);
extern std::unique_ptr<Ui> CreateUiWithFakeDisk(const Preferences& prefs,
    Timer& timer,
    std::unique_ptr<DiskSensor> fakeSensor,
    App& app);
