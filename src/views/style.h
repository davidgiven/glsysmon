#pragma once

#include <functional>
#include <string>
#include <vector>

class Style
{
public:
    static void GraphGroup(
        const std::string& title, std::function<void()> body);

    static void DrawGraph(const std::string& title,
        const std::string& subtitle,
        std::function<void()> body);
    static void DrawGraph(
        const std::vector<std::string>& titles, std::function<void()> body);
    static void DrawGraph(const std::string& title,
        const std::string& subtitle,
        int n,
        double yMin,
        double yMax,
        std::function<void()> body,
        float height = 40);
    static void DrawGraph(const std::vector<std::string>& titles,
        int n,
        double yMin,
        double yMax,
        std::function<void()> body,
        float height = 40);

    static void DrawCentredText(const std::string& text);

    static bool DrawToggleButton(const std::string& text, bool* state);
};
