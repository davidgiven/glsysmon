#pragma once

struct SDL_Window;

// UI backend interface. Implementations live in ui.cpp.
class Ui
{
public:
    virtual ~Ui() = default;

    // Polls sensors for all views. Called at the update rate.
    virtual void Tick() = 0;

    // Draws one frame of the widget into the window's swapchain.
    // Called at the redraw rate.
    virtual void Draw(SDL_Window* window, const char* backend) = 0;
};
