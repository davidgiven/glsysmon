#include "style.h"

#include <imgui.h>
#include <implot.h>

#include <algorithm>
#include <functional>
#include <string>
#include <vector>

namespace
{

    std::vector<std::string> splitLines(const std::string& s)
    {
        std::vector<std::string> lines;
        std::string cur;
        for (char c : s)
        {
            if (c == '\n')
            {
                lines.push_back(cur);
                cur.clear();
            }
            else
                cur.push_back(c);
        }
        lines.push_back(cur);
        return lines;
    }

    std::string joinLines(const std::vector<std::string>& titles)
    {
        std::string s;
        for (size_t i = 0; i < titles.size(); ++i)
        {
            if (i)
                s.push_back('\n');
            s += titles[i];
        }
        return s;
    }

} // namespace

void Style::GraphGroup(const std::string& title, std::function<void()> body)
{
    ImGui::Dummy(ImVec2(0.0f, ImGui::GetFontSize() * 0.5f));
    DrawCentredText(title);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
    ImPlot::PushStyleVar(ImPlotStyleVar_PlotPadding, ImVec2(0.0f, 0.0f));
    body();
    ImPlot::PopStyleVar();
    ImGui::PopStyleVar(2);
}

void Style::DrawGraph(const std::string& title,
    const std::string& subtitle,
    std::function<void()> body)
{
    DrawGraph(title, subtitle, 60, 0, 1, std::move(body));
}

void Style::DrawGraph(
    const std::vector<std::string>& titles, std::function<void()> body)
{
    DrawGraph(titles, 60, 0, 1, std::move(body));
}

void Style::DrawGraph(const std::string& title,
    const std::string& subtitle,
    int n,
    double yMin,
    double yMax,
    std::function<void()> body,
    float height)
{
    std::vector<std::string> titles;
    titles.reserve((title.empty() ? 0 : 1) +
                   std::count(subtitle.begin(), subtitle.end(), '\n') + 1);
    if (!title.empty())
        titles.push_back(title);
    if (!subtitle.empty())
    {
        std::vector<std::string> subLines = splitLines(subtitle);
        titles.insert(titles.end(), subLines.begin(), subLines.end());
    }
    DrawGraph(titles, n, yMin, yMax, std::move(body), height);
}

void Style::DrawGraph(const std::vector<std::string>& titles,
    int n,
    double yMin,
    double yMax,
    std::function<void()> body,
    float height)
{
    std::string joined = joinLines(titles);
    std::string plotId = std::string("##") + joined;
    const bool inverted = yMin > yMax;
    if (inverted)
        plotId += "_inv";
    const float width = ImGui::GetContentRegionAvail().x;
    if (inverted)
    {
        ImPlot::PushStyleColor(ImPlotCol_PlotBg, IM_COL32(0, 0, 0, 0));
        ImPlot::PushStyleColor(ImPlotCol_FrameBg, IM_COL32(0, 0, 0, 0));
    }
    if (ImPlot::BeginPlot(plotId.c_str(),
            ImVec2(width, height),
            ImPlotFlags_NoTitle | ImPlotFlags_NoLegend |
                ImPlotFlags_NoMouseText | ImPlotFlags_NoInputs |
                ImPlotFlags_NoMenus | ImPlotFlags_NoBoxSelect |
                ImPlotFlags_NoFrame))
    {
        ImPlotAxisFlags yFlags = ImPlotAxisFlags_NoDecorations;
        if (inverted)
            yFlags |= ImPlotAxisFlags_Invert;
        ImPlot::SetupAxes(
            nullptr, nullptr, ImPlotAxisFlags_NoDecorations, yFlags);
        ImPlot::SetupAxesLimits(0, n, yMin, yMax, ImPlotCond_Always);
        ImPlot::SetupFinish();
        body();
        ImVec2 pos = ImPlot::GetPlotPos();
        const float fontSize = ImGui::GetFontSize() * 2.0f / 3.0f;
        ImDrawList* drawList = ImPlot::GetPlotDrawList();
        ImFont* font = ImGui::GetFont();
        ImU32 col = ImGui::GetColorU32(ImGuiCol_Text);
        for (size_t i = 0; i < titles.size(); ++i)
        {
            if (titles[i].empty())
                continue;
            drawList->AddText(font,
                fontSize,
                ImVec2(pos.x + 2, pos.y + 2 + fontSize * static_cast<float>(i)),
                col,
                titles[i].c_str());
        }
        ImPlot::EndPlot();
    }
    if (inverted)
        ImPlot::PopStyleColor(2);
}

void Style::DrawCentredText(const std::string& text)
{
    const float avail = ImGui::GetContentRegionAvail().x;
    const float textWidth = ImGui::CalcTextSize(text.c_str()).x;
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (avail - textWidth) * 0.5f);
    ImGui::TextUnformatted(text.c_str());
}

bool Style::DrawToggleButton(const std::string& text, bool* v)
{
    bool clicked = false;

    bool oldv = *v;
    if (oldv)
    {
        ImVec4 color = ImGui::GetStyle().Colors[ImGuiCol_ButtonActive];
        ImGui::PushStyleColor(ImGuiCol_Button, color);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, color);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, color);
    }

    if (ImGui::Button(text.c_str()))
    {
        *v = !*v;
        clicked = true;
    }

    if (oldv)
        ImGui::PopStyleColor(3);

    return clicked;
}
