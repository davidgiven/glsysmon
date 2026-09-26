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

        std::vector<std::string> viewOrder =
            GlobalPreferencesFetcher::GetViews(*_pendingPreferences);
        {
            std::set<std::string> seen(viewOrder.begin(), viewOrder.end());
            for (View* v : _views.GetAllViews())
                if (!seen.contains(v->GetPrefName()))
                    viewOrder.push_back(v->GetPrefName());
        }

        for (size_t i = 0; i < viewOrder.size(); ++i)
        {
            View* view = _views.Get(viewOrder[i]);
            if (view == nullptr)
            {
                continue;
            }
            ImGui::PushID(view->GetPrefName().c_str());
            std::string enabledKey = view->GetPrefName() + ".enabled";
            bool enabled =
                _pendingPreferences->GetBoolean(enabledKey).value_or(true);
            bool isOpen = ImGui::CollapsingHeader(
                view->GetHumanName().c_str(), ImGuiTreeNodeFlags_AllowOverlap);
            float frameHeight = ImGui::GetFrameHeight();
            float itemSpacing = ImGui::GetStyle().ItemSpacing.x;
            float framePadding = ImGui::GetStyle().FramePadding.x;
            float totalWidth = frameHeight * 3 + itemSpacing * 2;
            ImGui::SameLine(
                ImGui::GetContentRegionMax().x - totalWidth - framePadding);

            bool upPressed = false;
            bool downPressed = false;
            if (i == 0)
                ImGui::BeginDisabled();
            upPressed = ImGui::Button((const char*) u8"↑", ImVec2(frameHeight, frameHeight));
            if (i == 0)
                ImGui::EndDisabled();
            ImGui::SameLine(0, itemSpacing);
            if (i + 1 >= viewOrder.size())
                ImGui::BeginDisabled();
            downPressed =
                ImGui::Button((const char*) u8"↓", ImVec2(frameHeight, frameHeight));
            if (i + 1 >= viewOrder.size())
                ImGui::EndDisabled();
            ImGui::SameLine(0, itemSpacing);
            if (ImGui::Checkbox("##enabled", &enabled))
                _pendingPreferences->SetBoolean(enabledKey, enabled);
            if (upPressed && i > 0)
            {
                std::vector<std::string> newOrder = viewOrder;
                std::swap(newOrder[i], newOrder[i - 1]);
                _pendingPreferences->SetStringList("views", newOrder);
            }
            if (downPressed && i + 1 < viewOrder.size())
            {
                std::vector<std::string> newOrder = viewOrder;
                std::swap(newOrder[i], newOrder[i + 1]);
                _pendingPreferences->SetStringList("views", newOrder);
            }

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
