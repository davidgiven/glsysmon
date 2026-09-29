#pragma once

class App;
class ImGuiIf;
class Preferences;
class Timer;

class Context
{
public:
    Context(App& app, ImGuiIf& imgui, Preferences& preferences, Timer& timer);

    App& app;
    ImGuiIf& imgui;
    Preferences& preferences;
    Timer& timer;
};
