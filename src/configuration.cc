#include "configuration.h"

#include <imgui.h>

#include "app.h"
#include "preferences/preferences.h"
#include "sensors/sensors.h"
#include "views/views.h"

ConfigurationWindow::ConfigurationWindow(
    const Views& views, const Sensors& sensors, App& app):
    _views(views),
    _sensors(sensors),
    _app(app),
    _pendingPreferences(CreateCombinedPreferences(
        {CreateMapPreferences(), app.GetPreferences()}))
{
}

void ConfigurationWindow::Draw()
{
    float buttonHeight = ImGui::GetFrameHeightWithSpacing();

    if (ImGui::BeginChild("UpperArea", ImVec2(0, -buttonHeight), false))
    {
        if (ImGui::BeginTabBar("ConfigurationTabs"))
        {
            if (ImGui::BeginTabItem("Views"))
            {
                for (View* view : _views.GetAllViews())
                    if (ImGui::CollapsingHeader(view->GetHumanName().c_str()))
                    {
                        ImGui::PushID("view");
                        ImGui::PushID(view->GetPrefName().c_str());
                        view->DrawConfiguration(*_pendingPreferences);
                        ImGui::PopID();
                        ImGui::PopID();
                    }
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Sensors"))
            {
                for (Sensor* sensor : _sensors.GetAllSensors())
                    if (ImGui::CollapsingHeader(sensor->GetHumanName().c_str()))
                    {
                        ImGui::PushID("sensor");
                        ImGui::PushID(sensor->GetPrefName().c_str());
                        sensor->DrawConfiguration(*_pendingPreferences);
                        ImGui::PopID();
                        ImGui::PopID();
                    }
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
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
        if (ImGui::Button("OK")) {}
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {}
    }
    ImGui::EndChild();
}
