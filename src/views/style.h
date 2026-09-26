#pragma once

#include <functional>
#include <string>

class Style
{
public:
    static void GraphGroup(const char* title, std::function<void()> body);
    static void GraphGroup(
        const std::string& title, std::function<void()> body);

    static void DrawGraph(const char* title, std::function<void()> body);
    static void DrawGraph(const std::string& title, std::function<void()> body);
    static void DrawGraph(const char* title,
        int n,
        double yMin,
        double yMax,
        std::function<void()> body,
        float height = 40,
        bool yAuto = false);
    static void DrawGraph(const std::string& title,
        int n,
        double yMin,
        double yMax,
        std::function<void()> body,
        float height = 40,
        bool yAuto = false);

    static void DrawCentredText(const char* text);
    static void DrawCentredText(const std::string& text);

    static bool DrawToggleButton(const char* text, bool* state);
    static bool DrawToggleButton(const std::string& text, bool* state);
};
