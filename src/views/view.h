#pragma once

#include "app.h"

#include <memory>
#include <string>
#include <vector>

class App;
class ImGuiIf;
class Preferences;
class Sensor;
class Sensors;
class ClockSensor;
class CpuSensor;
class DiskSensor;
class HostnameSensor;
class MemorySensor;
class NetworkSensor;
class TemperatureSensor;

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

    virtual std::vector<Sensor*> GetSensors() = 0;

    virtual std::vector<Sensor*> GetSensors() const = 0;

    virtual ImGuiIf& GetImGui() = 0;
    virtual const ImGuiIf& GetImGui() const = 0;
};

extern std::unique_ptr<View> CreateClockView(
    App& app, Sensors& sensors);
extern std::unique_ptr<View> CreateClockView(
    App& app, std::unique_ptr<ClockSensor> sensor);
extern std::unique_ptr<View> CreateCpuView(
    App& app, Sensors& sensors);
extern std::unique_ptr<View> CreateCpuView(
    App& app, std::unique_ptr<CpuSensor> sensor);
extern std::unique_ptr<View> CreateHostnameView(
    App& app, Sensors& sensors);
extern std::unique_ptr<View> CreateHostnameView(
    App& app, std::unique_ptr<HostnameSensor> sensor);
extern std::unique_ptr<View> CreateTemperatureView(
    App& app, Sensors& sensors);
extern std::unique_ptr<View> CreateTemperatureView(
    App& app, std::unique_ptr<TemperatureSensor> sensor);
extern std::unique_ptr<View> CreateNetworkView(
    App& app, Sensors& sensors);
extern std::unique_ptr<View> CreateNetworkView(
    App& app, std::unique_ptr<NetworkSensor> sensor);
extern std::unique_ptr<View> CreateDiskView(
    App& app, Sensors& sensors);
extern std::unique_ptr<View> CreateDiskView(
    App& app, std::unique_ptr<DiskSensor> sensor);
extern std::unique_ptr<View> CreateMemoryView(
    App& app, Sensors& sensors);
extern std::unique_ptr<View> CreateMemoryView(
    App& app, std::unique_ptr<MemorySensor> sensor);
extern std::unique_ptr<View> CreateBubbleFishyMonView(
    App& app, Sensors& sensors);
