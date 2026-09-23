#include "configuration.h"

#include <imgui.h>

void ConfigurationWindow::Draw()
{
    float buttonHeight = ImGui::GetFrameHeightWithSpacing();

    if (ImGui::BeginChild("UpperArea", ImVec2(0, -buttonHeight), false))
    {
        if (ImGui::BeginTabBar("ConfigurationTabs"))
        {
            if (ImGui::BeginTabItem("General"))
            {
                ImGui::Text("General settings");
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Views"))
            {
                ImGui::Text("Views settings");
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Advanced"))
            {
                ImGui::Text("Advanced settings");
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
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() +
                             ImGui::GetContentRegionAvail().x - totalWidth);
        if (ImGui::Button("OK")) {}
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {}
    }
    ImGui::EndChild();
}
