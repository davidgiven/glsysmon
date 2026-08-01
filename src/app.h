#pragma once

#include "dock.h"

// Application entry interface. Implementations live in app.cpp.
class App {
public:
    virtual ~App() = default;

    // Bootstraps SDL/ImGui, runs the main loop, returns process exit code.
    virtual int Run(const DockConfig &cfg) = 0;
};
