#pragma once

struct SDL_Window;

// UI backend interface. Implementations live in ui.cpp.
class Ui
{
public:
    virtual ~Ui() = default;

    // Draws one frame of the widget into the window's swapchain.
    virtual void Draw(SDL_Window* window, const char* backend) = 0;
};
