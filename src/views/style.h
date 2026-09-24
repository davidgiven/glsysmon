#pragma once

#include <functional>
#include <string>

class Style
{
public:
    static void GraphGroup(const char* title, std::function<void()> body);
    static void GraphGroup(
        const std::string& title, std::function<void()> body);

    static void DrawCentredText(const char* text);
    static void DrawCentredText(const std::string& text);
};
