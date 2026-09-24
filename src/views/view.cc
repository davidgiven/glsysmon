#include "views/view.h"

#include <imgui.h>

#include "preferences/preferences.h"

void View::DrawConfiguration(Preferences& preferences)
{
    (void)preferences;

    bool enabled = false;
    ImGui::Checkbox("Enabled", &enabled);
}
