#pragma once

#include <memory>

#include "app.h"
#include "dock.h"
#include "imgui_frame_renderer.h"
#include "preferences.h"
#include "sensors/clock_sensor.h"
#include "sensors/hostname_sensor.h"
#include "sensors/sensors.h"
#include "ui.h"
#include "view.h"
#include "views/catalogue.h"

extern std::unique_ptr<Preferences> CreateCliPreferences(const CliArgs& args);
extern std::unique_ptr<Preferences> CreateTomlPreferences();
extern std::unique_ptr<Preferences> CreatePreferences(const CliArgs& args);

extern std::unique_ptr<Dock> CreateDock(const Preferences& prefs);
extern DockFactory CreateDockFactory(const Preferences& prefs);

extern std::unique_ptr<ClockSensor> CreateClockSensor();
extern std::unique_ptr<HostnameSensor> CreateHostnameSensor();
extern std::unique_ptr<View> CreateClockView(Sensors& sensors);
extern std::unique_ptr<View> CreateClockView();
extern std::unique_ptr<View> CreateClockView(
    std::unique_ptr<ClockSensor> sensor);
extern std::unique_ptr<View> CreateHostnameView(Sensors& sensors);
extern std::unique_ptr<View> CreateHostnameView();
extern std::unique_ptr<View> CreateHostnameView(
    std::unique_ptr<HostnameSensor> sensor);

extern std::unique_ptr<Ui> CreateUi(const Preferences& prefs);
extern std::unique_ptr<Ui> CreateUiWithFakeHostname(
    const Preferences& prefs, std::unique_ptr<HostnameSensor> fakeSensor);
extern std::unique_ptr<Ui> CreateUiWithFakeClock(
    const Preferences& prefs, std::unique_ptr<ClockSensor> fakeSensor);
extern std::unique_ptr<ImGuiFrameRenderer> CreateImGuiFrameRenderer();
extern std::unique_ptr<App> CreateApp(const CliArgs& args);
