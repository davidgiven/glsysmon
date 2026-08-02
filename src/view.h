#pragma once

// A system-monitor widget shown in the dock. Implementations live in
// *_view_impl.cc.
class View
{
public:
    virtual ~View() = default;

    // Redraws the view into the active ImGui window.
    virtual void Tick() = 0;
};
