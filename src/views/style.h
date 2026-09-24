#pragma once

#include <string>

class Style
{
public:
    class GraphGroup
    {
    public:
        explicit GraphGroup(const char* title);
        explicit GraphGroup(const std::string& title);
        ~GraphGroup();

        GraphGroup(const GraphGroup&) = delete;
        GraphGroup& operator=(const GraphGroup&) = delete;
    };

    static void DrawCentredText(const char* text);
    static void DrawCentredText(const std::string& text);
};
