#include "style.h"

#include <imgui.h>
#include <implot.h>

#include <functional>
#include <string>

void Style::GraphGroup(const char* title, std::function<void()> body)
{
    DrawCentredText(title);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
    ImPlot::PushStyleVar(ImPlotStyleVar_PlotPadding, ImVec2(0.0f, 0.0f));
    body();
    ImPlot::PopStyleVar();
    ImGui::PopStyleVar(2);
}

void Style::GraphGroup(const std::string& title, std::function<void()> body)
{
    GraphGroup(title.c_str(), std::move(body));
}

void Style::DrawCentredText(const char* text)
{
    const float avail = ImGui::GetContentRegionAvail().x;
    const float textWidth = ImGui::CalcTextSize(text).x;
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (avail - textWidth) * 0.5f);
    ImGui::TextUnformatted(text);
}

void Style::DrawCentredText(const std::string& text)
{
    DrawCentredText(text.c_str());
}
