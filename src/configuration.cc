#include "configuration.h"

#include <imgui.h>

#include <cstdio>
#include <csetjmp>

#include "app.h"
#include "preferences/preferences.h"
#include "restart.h"
#include "sensors/sensor.h"
#include "views/views.h"

jmp_buf g_restartJmp;

ConfigurationWindow::ConfigurationWindow(const Views& views, App& app):
    _views(views),
    _app(app),
    _pendingMapPreferences(CreateMapPreferences()),
    _pendingPreferences(CreateCombinedPreferences(
        {_pendingMapPreferences, app.GetPreferences()}))
{
}

void ConfigurationWindow::Draw(bool* open)
{
    float buttonHeight = ImGui::GetFrameHeightWithSpacing();

    if (ImGui::BeginChild("UpperArea", ImVec2(0, -buttonHeight), false))
    {
        for (View* view : _views.GetAllViews())
            if (ImGui::CollapsingHeader(view->GetHumanName().c_str()))
            {
                ImGui::PushID(view->GetPrefName().c_str());
                view->DrawConfiguration(*_pendingPreferences);

                ImGui::PushID("sensors");
                for (Sensor* sensor : view->GetSensors())
                {
                    ImGui::PushID(sensor->GetPrefName().c_str());
                    sensor->DrawConfiguration(*_pendingPreferences);
                    ImGui::PopID();
                }

                ImGui::PopID();
                ImGui::PopID();
            }
    }
    ImGui::EndChild();

    if (ImGui::BeginChild("LowerArea", ImVec2(0, 0), false))
    {
        float okWidth = ImGui::CalcTextSize("OK").x +
                        ImGui::GetStyle().FramePadding.x * 2.0f;
        float cancelWidth = ImGui::CalcTextSize("Cancel").x +
                            ImGui::GetStyle().FramePadding.x * 2.0f;
        float totalWidth =
            okWidth + cancelWidth + ImGui::GetStyle().ItemSpacing.x;
        ImGui::SetCursorPosX(0);
        if (ImGui::Button("Quit!"))
            _app.Quit();
        ImGui::SameLine();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() +
                             ImGui::GetContentRegionAvail().x - totalWidth);
        if (ImGui::Button("OK"))
        {
            WriteTomlPreferences(*_pendingPreferences);
            longjmp(g_restartJmp, 1);
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel"))
        {
            _pendingMapPreferences->ClearAll();
            if (open != nullptr)
                *open = false;
        }
    }
    ImGui::EndChild();
}
