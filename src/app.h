#pragma once

// Application entry interface. Implementations live in imgui_app_impl.cc.
// The main loop lives in main.cc, which drives Setup()/Tick()/Shutdown().
class App {
public:
    virtual ~App() = default;

    // Initializes SDL, the dock, the GPU device and ImGui. Returns 0 on
    // success, a non-zero exit code on failure (partial resources released).
    virtual int Setup() = 0;

    // Runs one frame: pumps events, updates and draws the widget. Returns
    // false when the app should shut down.
    virtual bool Tick() = 0;

    // Releases all resources acquired by Setup().
    virtual void Shutdown() = 0;
};
