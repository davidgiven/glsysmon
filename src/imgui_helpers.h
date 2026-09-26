#pragma once

#include <imgui.h>

class ImguiDisabled
{
public:
    explicit ImguiDisabled(bool disabled): _disabled(disabled)
    {
        if (_disabled)
            ImGui::BeginDisabled();
    }

    ~ImguiDisabled()
    {
        if (_disabled)
            ImGui::EndDisabled();
    }

    ImguiDisabled(const ImguiDisabled&) = delete;
    ImguiDisabled& operator=(const ImguiDisabled&) = delete;

private:
    bool _disabled;
};
