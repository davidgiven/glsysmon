#pragma once

#include <map>
#include "preferences/preferences.h"

class App;
class Sensors;
class Views;

class ConfigurationWindow
{
public:
    ConfigurationWindow(const Views& views, const Sensors& sensors, App& app);

    void Draw();

private:
    const Views& _views;
    const Sensors& _sensors;
    App& _app;
    std::unique_ptr<Preferences> _pendingPreferences;
};
