#pragma once

#include "app.h"

#include <string>

class App;
class ImGuiIf;
class Preferences;

// Fetches a piece of system data. Implementations live in sensor_*.cc.
class Sensor
{
public:
    explicit Sensor(App& app, const std::string& prefPrefix):
        _app(&app),
        _prefPrefix(prefPrefix)
    {
    }

    explicit Sensor(const std::string& prefPrefix):
        _app(nullptr),
        _prefPrefix(prefPrefix)
    {
    }

    virtual ~Sensor() = default;

    virtual std::string GetHumanName() const = 0;

    virtual std::string GetPrefName() const = 0;

    virtual void DrawConfiguration(Preferences& preferences);

    virtual ImGuiIf& GetImGui();
    virtual const ImGuiIf& GetImGui() const;

protected:
    App* _app = nullptr;
    std::string _prefPrefix;
};
