#include "views/view.h"

#include <imgui.h>

#include "preferences/preferences.h"

void View::DrawConfiguration(Preferences& preferences)
{
    // Enabled
    std::string key = GetPrefName() + ".enabled";
    bool enabled = preferences.GetBoolean(key).value_or(true);
    if (ImGui::Checkbox("Enabled", &enabled))
        preferences.SetBoolean(key, enabled);
}
