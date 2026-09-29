#include "mock_imgui_lib.h"

MockImGuiIf::MockImGuiIf()
{
    if (ImGui::GetCurrentContext() == nullptr)
    {
        ImGui::CreateContext();
        ImPlot::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.Fonts->AddFontDefault();
        unsigned char* pixels = nullptr;
        int w = 0, h = 0;
        io.Fonts->GetTexDataAsRGBA32(&pixels, &w, &h);
    }
    _dummyDrawListShared = new ImDrawListSharedData();
    _dummyDrawList = new ImDrawList(_dummyDrawListShared);
    _style = ImGuiStyle();
    _dummyVec4 = ImVec4(1, 1, 1, 1);
    if (ImGui::GetCurrentContext() != nullptr)
    {
        ImGuiIO& io = ImGui::GetIO();
        if (!io.Fonts->Fonts.empty())
            _font = io.Fonts->Fonts[0];
    }
}

MockImGuiIf::~MockImGuiIf()
{
    delete _dummyDrawList;
    delete _dummyDrawListShared;
}