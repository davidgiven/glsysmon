#pragma once

#include "app.h"

#include <map>
#include "preferences/preferences.h"

class App;
class ImGuiIf;
class Views;

class ConfigurationWindow
{
public:
    ConfigurationWindow(const Views& views, App& app, ImGuiIf& imgui);

    void Draw(bool* open = nullptr);

private:
    const Views& _views;
    App& _app;
    ImGuiIf& _imgui;
    std::shared_ptr<Preferences> _pendingMapPreferences;
    std::unique_ptr<Preferences> _pendingPreferences;
    int _openSection = 0;

    void DrawGlobalConfiguration();
};
