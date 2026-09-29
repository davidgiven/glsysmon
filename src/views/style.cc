#include "style.h"

#include "imguiif.h"
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

void Style::GraphGroup(
    ImGuiIf& imgui, const std::string& title, std::function<void()> body)
{
    imgui.Dummy(ImVec2(0.0f, imgui.GetFontSize() * 0.5f));
    DrawCentredText(imgui, title);
    imgui.PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
    imgui.PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
    imgui.ImPlot_PushStyleVar(ImPlotStyleVar_PlotPadding, ImVec2(0.0f, 0.0f));
    body();
    imgui.ImPlot_PopStyleVar();
    imgui.PopStyleVar(2);
}

void Style::DrawGraph(ImGuiIf& imgui,
    const std::string& title,
    const std::string& subtitle,
    std::function<void()> body,
    float labelFontSize)
{
    DrawGraph(imgui, title, subtitle, 60, 0, 1, std::move(body), labelFontSize);
}

void Style::DrawGraph(ImGuiIf& imgui,
    const std::vector<std::string>& titles,
    std::function<void()> body,
    float labelFontSize)
{
    DrawGraph(imgui, titles, 60, 0, 1, std::move(body), labelFontSize);
}

void Style::DrawGraph(ImGuiIf& imgui,
    const std::string& title,
    const std::string& subtitle,
    int n,
    double yMin,
    double yMax,
    std::function<void()> body,
    float labelFontSize,
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
    DrawGraph(
        imgui, titles, n, yMin, yMax, std::move(body), labelFontSize, height);
}

void Style::DrawGraph(ImGuiIf& imgui,
    const std::vector<std::string>& titles,
    int n,
    double yMin,
    double yMax,
    std::function<void()> body,
    float labelFontSize,
    float height)
{
    std::string joined = joinLines(titles);
    std::string plotId = std::string("##") + joined;
    const bool inverted = yMin > yMax;
    if (inverted)
        plotId += "_inv";
    const float width = imgui.GetContentRegionAvail().x;
    if (inverted)
    {
        imgui.ImPlot_PushStyleColor(ImPlotCol_PlotBg, IM_COL32(0, 0, 0, 0));
        imgui.ImPlot_PushStyleColor(ImPlotCol_FrameBg, IM_COL32(0, 0, 0, 0));
    }
    if (imgui.BeginPlot(plotId.c_str(),
            ImVec2(width, height),
            ImPlotFlags_NoTitle | ImPlotFlags_NoLegend |
                ImPlotFlags_NoMouseText | ImPlotFlags_NoInputs |
                ImPlotFlags_NoMenus | ImPlotFlags_NoBoxSelect |
                ImPlotFlags_NoFrame))
    {
        ImPlotAxisFlags yFlags = ImPlotAxisFlags_NoDecorations;
        if (inverted)
            yFlags |= ImPlotAxisFlags_Invert;
        imgui.SetupAxes(
            nullptr, nullptr, ImPlotAxisFlags_NoDecorations, yFlags);
        imgui.SetupAxesLimits(0, n, yMin, yMax, ImPlotCond_Always);
        imgui.SetupFinish();
        body();
        ImVec2 pos = imgui.GetPlotPos();
        const float fontSize = labelFontSize;
        ImDrawList* drawList = imgui.GetPlotDrawList();
        ImFont* font = imgui.GetFont();
        ImU32 col = imgui.GetColorU32(ImGuiCol_Text);
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
        imgui.EndPlot();
    }
    if (inverted)
        imgui.ImPlot_PopStyleColor(2);
}

void Style::DrawCentredText(ImGuiIf& imgui, const std::string& text)
{
    const float avail = imgui.GetContentRegionAvail().x;
    const float textWidth = imgui.CalcTextSize(text.c_str()).x;
    imgui.SetCursorPosX(imgui.GetCursorPosX() + (avail - textWidth) * 0.5f);
    imgui.TextUnformatted(text.c_str());
}

bool Style::DrawToggleButton(ImGuiIf& imgui, const std::string& text, bool* v)
{
    bool clicked = false;

    bool oldv = *v;
    if (oldv)
    {
        ImVec4 color = imgui.GetStyle().Colors[ImGuiCol_ButtonActive];
        imgui.PushStyleColor(ImGuiCol_Button, color);
        imgui.PushStyleColor(ImGuiCol_ButtonHovered, color);
        imgui.PushStyleColor(ImGuiCol_ButtonActive, color);
    }

    if (imgui.Button(text.c_str()))
    {
        *v = !*v;
        clicked = true;
    }

    if (oldv)
        imgui.PopStyleColor(3);

    return clicked;
}
