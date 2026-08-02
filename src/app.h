#pragma once

// Application entry interface. Implementations live in imgui_app_impl.cc.
class App {
public:
    virtual ~App() = default;

    // Bootstraps SDL/ImGui, runs the main loop, returns process exit code.
    virtual int Run() = 0;
};
