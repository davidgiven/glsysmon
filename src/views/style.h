#pragma once

#include <functional>
#include <string>
#include <vector>

class ImGuiIf;

class Style
{
public:
    static void GraphGroup(
        ImGuiIf& imgui, const std::string& title, std::function<void()> body);

    static void DrawGraph(ImGuiIf& imgui,
        const std::string& title,
        const std::string& subtitle,
        std::function<void()> body);
    static void DrawGraph(ImGuiIf& imgui,
        const std::vector<std::string>& titles, std::function<void()> body);
    static void DrawGraph(ImGuiIf& imgui,
        const std::string& title,
        const std::string& subtitle,
        int n,
        double yMin,
        double yMax,
        std::function<void()> body,
        float height = 40);
    static void DrawGraph(ImGuiIf& imgui,
        const std::vector<std::string>& titles,
        int n,
        double yMin,
        double yMax,
        std::function<void()> body,
        float height = 40);

    static void DrawCentredText(ImGuiIf& imgui, const std::string& text);

    static bool DrawToggleButton(ImGuiIf& imgui, const std::string& text, bool* state);
};
