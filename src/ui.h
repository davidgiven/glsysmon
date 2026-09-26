#pragma once

#include <memory>

class Preferences;
class Timer;
class Sensors;
class App;

struct SDL_Window;

// UI backend interface. Implementations live in ui.cpp.
class Ui
{
public:
    virtual ~Ui() = default;

    virtual void Draw() = 0;
};

extern std::unique_ptr<Ui> CreateUi(
    const Preferences& prefs, Timer& timer, App& app);
extern std::unique_ptr<Ui> CreateUi(
    const Preferences& prefs, Sensors& sensors, App& app);
