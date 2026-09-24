#pragma once

#include <memory>
#include <string>

class Preferences;
class Sensors;
class HostnameSensor;

// A system-monitor widget shown in the dock. Implementations live in
// *_view_impl.cc.
class View
{
public:
    virtual ~View() = default;

    // Redraws the view into the active ImGui window by fetching data from
    // its sensor. Called at the redraw rate.
    virtual void Draw() = 0;

    virtual void DrawConfiguration(Preferences& preferences);

    virtual std::string GetHumanName() const = 0;

    virtual std::string GetPrefName() const = 0;
};

extern std::unique_ptr<View> CreateClockView(
    const Preferences& prefs, Sensors& sensors);
extern std::unique_ptr<View> CreateCpuView(
    const Preferences& prefs, Sensors& sensors);
extern std::unique_ptr<View> CreateHostnameView(
    const Preferences& prefs, Sensors& sensors);
extern std::unique_ptr<View> CreateHostnameView(
    const Preferences& prefs, std::unique_ptr<HostnameSensor> sensor);
extern std::unique_ptr<View> CreateTemperatureView(
    const Preferences& prefs, Sensors& sensors);
