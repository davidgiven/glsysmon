#pragma once

// Application entry interface. Implementations live in imgui_app_impl.cc.
// The main loop lives in main.cc, which drives Setup()/Tick()/Shutdown().
class App
{
public:
    virtual ~App() = default;

    // Initializes SDL, the dock, the GPU device and ImGui. Throws
    // std::runtime_error on failure; resources acquired before the failure are
    // released via RAII when the App is destroyed.
    virtual void Setup() = 0;

    // Runs one frame: pumps events, updates and draws the widget. Returns
    // false when the app should shut down.
    virtual bool Tick() = 0;

    // Releases all resources acquired by Setup(). Safe to call multiple times;
    // the destructor calls it again as a safety net.
    virtual void Shutdown() = 0;
};
