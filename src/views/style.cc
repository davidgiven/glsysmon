#include "style.h"

#include <imgui.h>
#include <implot.h>

#include <string>

Style::GraphGroup::GraphGroup(const char* title)
{
    DrawCentredText(title);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
    ImPlot::PushStyleVar(ImPlotStyleVar_PlotPadding, ImVec2(0.0f, 0.0f));
}

Style::GraphGroup::GraphGroup(const std::string& title):
    GraphGroup(title.c_str())
{
}

Style::GraphGroup::~GraphGroup()
{
    ImPlot::PopStyleVar();
    ImGui::PopStyleVar(2);
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
