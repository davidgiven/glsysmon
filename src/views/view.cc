#include "views/view.h"

#include <imgui.h>

#include "preferences/preferences.h"

void View::DrawConfiguration(Preferences& preferences)
{
    std::string key = GetPrefName() + ".enabled";
    bool enabled = true;
    if (auto value = preferences.GetString(key))
        enabled = (*value == "true" || *value == "1");
    else if (auto iv = preferences.GetInteger(key))
        enabled = (*iv != 0);
    if (ImGui::Checkbox("Enabled", &enabled))
        preferences.SetString(key, enabled ? "true" : "false");
}
