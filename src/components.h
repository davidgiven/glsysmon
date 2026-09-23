#pragma once

#include <initializer_list>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "app.h"
#include "display/dock.h"
#include "display/imgui_frame_renderer.h"
#include "preferences/preferences.h"
#include "sensors/clock_sensor.h"
#include "sensors/cpu_sensor.h"
#include "sensors/hostname_sensor.h"
#include "sensors/temperature_sensor.h"
#include "sensors/sensors.h"
#include "timer.h"
#include "ui.h"
#include "views/view.h"
#include "views/views.h"

extern std::unique_ptr<Preferences> CreateCliPreferences(const CliArgs& args);
extern std::unique_ptr<Preferences> CreateTomlPreferences();
extern std::unique_ptr<Preferences> CreateDefaultPreferences();
extern std::shared_ptr<Preferences> CreateMapPreferences(
    const std::map<std::string, std::string>& values);
extern std::shared_ptr<Preferences> CreateMapPreferences();
extern std::unique_ptr<Preferences> CreateCombinedPreferences(
    std::initializer_list<std::shared_ptr<Preferences>> sources);
extern std::unique_ptr<Preferences> CreateCombinedPreferences(
    std::vector<std::shared_ptr<Preferences>> sources);
extern std::unique_ptr<Preferences> CreatePreferences(const CliArgs& args);

extern std::unique_ptr<Dock> CreateDock(const Preferences& prefs);
extern DockFactory CreateDockFactory(const Preferences& prefs);

extern std::unique_ptr<ClockSensor> CreateClockSensor(
    const Preferences& prefs, Timer& timer);
extern std::unique_ptr<CpuSensor> CreateCpuSensor(
    const Preferences& prefs, Timer& timer, const std::string& procStatPath);
extern std::unique_ptr<CpuSensor> CreateCpuSensor(
    const Preferences& prefs, Timer& timer);
extern std::unique_ptr<HostnameSensor> CreateHostnameSensor(
    const Preferences& prefs, Timer& timer);
extern std::unique_ptr<TemperatureSensor> CreateTemperatureSensor(
    const Preferences& prefs, Timer& timer, const std::string& hwmonRoot);
extern std::unique_ptr<TemperatureSensor> CreateTemperatureSensor(
    const Preferences& prefs, Timer& timer);
extern std::unique_ptr<View> CreateClockView(
    const Preferences& prefs, Sensors& sensors);
extern std::unique_ptr<View> CreateClockView(
    const Preferences& prefs, std::unique_ptr<ClockSensor> sensor);
extern std::unique_ptr<View> CreateCpuView(
    const Preferences& prefs, Sensors& sensors);
extern std::unique_ptr<View> CreateCpuView(
    const Preferences& prefs, std::unique_ptr<CpuSensor> sensor);
extern std::unique_ptr<View> CreateHostnameView(
    const Preferences& prefs, Sensors& sensors);
extern std::unique_ptr<View> CreateHostnameView(
    const Preferences& prefs, std::unique_ptr<HostnameSensor> sensor);
extern std::unique_ptr<View> CreateTemperatureView(
    const Preferences& prefs, Sensors& sensors);
extern std::unique_ptr<View> CreateTemperatureView(
    const Preferences& prefs, std::unique_ptr<TemperatureSensor> sensor);

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
extern std::unique_ptr<ImGuiFrameRenderer> CreateImGuiFrameRenderer();
extern std::unique_ptr<Timer> CreateTimer();
extern std::unique_ptr<App> CreateApp(const CliArgs& args);
