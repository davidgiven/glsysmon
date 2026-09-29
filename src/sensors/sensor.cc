#include "sensors/sensor.h"

#include "context.h"
#include "imguiif.h"
#include "preferences/preferences.h"

void Sensor::DrawConfiguration(Preferences& preferences)
{
    (void)preferences;
}

ImGuiIf& Sensor::GetImGui()
{
    return _ctx->imgui;
}

const ImGuiIf& Sensor::GetImGui() const
{
    return _ctx->imgui;
}
