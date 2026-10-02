#pragma once

#include <memory>

class Context;
class Sensors;

struct SDL_Window;

// UI backend interface. Implementations live in ui.cpp.
class Ui
{
public:
    virtual ~Ui() = default;

    virtual void Draw() = 0;
};

extern std::unique_ptr<Ui> CreateUi(const Context& ctx, Sensors& sensors);
