#pragma once

#include <string>

// A system-monitor widget shown in the dock. Implementations live in
// *_view_impl.cc.
class View
{
public:
    virtual ~View() = default;

    // Redraws the view into the active ImGui window by fetching data from
    // its sensor. Called at the redraw rate.
    virtual void Draw() = 0;

    virtual std::string GetName() const = 0;
};
