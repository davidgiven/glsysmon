#include "configuration.h"

#include <imgui.h>

#include <cstdio>
#include <csetjmp>

#include "app.h"
#include "preferences/preferences.h"
#include "globals.h"
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

void ConfigurationWindow::DrawGlobalConfiguration()
{
    // Panel width
    int size = _pendingPreferences->GetInteger("size").value_or(100);
    if (ImGui::SliderInt("Width", &size, 1, 300))
    {
        _pendingPreferences->SetInteger("size", size);
    }

    // Dock side
    static constexpr const char* kSideLabels[] = {"left", "right"};
    static constexpr const char* kSideValues[] = {"left", "right"};
    std::string currentSide =
        _pendingPreferences->GetString("side").value_or("left");
    int sideIndex = indexOf(kSideValues, currentSide).value_or(0);

    if (ImGui::SliderInt("Side", &sideIndex, 0, 1, kSideLabels[sideIndex]))
    {
        _pendingPreferences->SetString("side", kSideValues[sideIndex]);
    }
}

void ConfigurationWindow::Draw(bool* open)
{
    float buttonHeight = ImGui::GetFrameHeightWithSpacing();

    if (ImGui::BeginChild("UpperArea", ImVec2(0, -buttonHeight), false))
    {
        if (ImGui::CollapsingHeader("Global"))
        {
            DrawGlobalConfiguration();
        }

        for (View* view : _views.GetAllViews())
        {
            ImGui::PushID(view->GetPrefName().c_str());
            std::string enabledKey = view->GetPrefName() + ".enabled";
            bool enabled =
                _pendingPreferences->GetBoolean(enabledKey).value_or(true);
            bool isOpen = ImGui::CollapsingHeader(
                view->GetHumanName().c_str(), ImGuiTreeNodeFlags_AllowOverlap);
            ImGui::SameLine(ImGui::GetContentRegionMax().x -
                            ImGui::GetFrameHeight() -
                            ImGui::GetStyle().FramePadding.x);
            if (ImGui::Checkbox("##enabled", &enabled))
                _pendingPreferences->SetBoolean(enabledKey, enabled);
            if (isOpen)
            {
                view->DrawConfiguration(*_pendingPreferences);

                ImGui::PushID("sensors");
                for (Sensor* sensor : view->GetSensors())
                {
                    ImGui::PushID(sensor->GetPrefName().c_str());
                    sensor->DrawConfiguration(*_pendingPreferences);
                    ImGui::PopID();
                }

                ImGui::PopID();
            }
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
