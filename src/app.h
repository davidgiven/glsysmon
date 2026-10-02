#pragma once

#include <memory>
#include "preferences/preferences.h"

class Timer;
class ImGuiIf;

// Application entry interface. Implementations live in imgui_app_impl.cc.
// The main loop lives in main.cc, which drives Setup()/MainLoop()/Shutdown().
class App
{
public:
    virtual ~App() = default;

    // Initializes SDL, the dock, the GPU device and ImGui. Throws
    // std::runtime_error on failure; resources acquired before the failure are
    // released via RAII when the App is destroyed.
    virtual void Setup() = 0;

    // Processes SDL events until the app should shut down and triggers redraws
    // via a dedicated Redraw method, which is fired either when a timer fires
    // or when required by the windowing system, capped by the fps preference.
    virtual void MainLoop() = 0;

    // Releases all resources acquired by Setup(). Safe to call multiple times;
    // the destructor calls it again as a safety net.
    virtual void Shutdown() = 0;

    // Return the current preferences object.
    virtual std::shared_ptr<Preferences> GetSharedPreferences() = 0;
    virtual Preferences& GetPreferences() = 0;
    virtual const Preferences& GetPreferences() const = 0;

    virtual Timer& GetTimer() = 0;
    virtual ImGuiIf& GetImGui() = 0;
    virtual const Timer& GetTimer() const = 0;
    virtual const ImGuiIf& GetImGui() const = 0;

    virtual void Quit() = 0;
};

extern std::unique_ptr<App> CreateApp(const CliArgs& args);
