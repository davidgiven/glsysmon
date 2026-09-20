#pragma once

// A system-monitor widget shown in the dock. Implementations live in
// *_view_impl.cc.
class View
{
public:
    virtual ~View() = default;

    // Polls sensors and caches data for the next Draw(). Called at the
    // update rate.
    virtual void Tick() = 0;

    // Redraws the view into the active ImGui window using cached data.
    // Called at the redraw rate.
    virtual void Draw() = 0;
};
