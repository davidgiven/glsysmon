#pragma once

struct SDL_Window;

// UI backend interface. Implementations live in ui.cpp.
class Ui
{
public:
    virtual ~Ui() = default;

    virtual void Draw() = 0;
};
