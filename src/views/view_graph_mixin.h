#pragma once

#include <imgui.h>

#include <string>

#include "preferences/preferences.h"
#include "views/view.h"

class ViewGraphMixin : public View
{
public:
    void DrawConfiguration(Preferences& preferences) override
    {
        View::DrawConfiguration(preferences);

        // Graph height
        std::string key = GetPrefName() + ".graph_height";
        int height = preferences.GetInteger(key).value_or(40);
        if (ImGui::InputInt("Graph height", &height))
        {
            if (height < 1)
                height = 1;
            preferences.SetInteger(key, height);
        }
    }
};
