#include "sensors/sensor.h"

#include "imguiif.h"
#include "preferences/preferences.h"

void Sensor::DrawConfiguration(Preferences& preferences)
{
    (void)preferences;
}

ImGuiIf& Sensor::GetImGui()
{
    return _app->GetImGui();
}

const ImGuiIf& Sensor::GetImGui() const
{
    return _app->GetImGui();
}
