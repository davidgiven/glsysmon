#pragma once

#include <imgui.h>

#include "imguiif.h"

#if __has_include(<magic_enum/magic_enum.hpp>)
#include <magic_enum/magic_enum.hpp>
#else
#include <magic_enum.hpp>
#endif

#include <string>

class ImguiDisabled
{
public:
    explicit ImguiDisabled(bool disabled): _disabled(disabled)
    {
        if (_disabled)
            ImGui::BeginDisabled();
    }

    ~ImguiDisabled()
    {
        if (_disabled)
            ImGui::EndDisabled();
    }

    ImguiDisabled(const ImguiDisabled&) = delete;
    ImguiDisabled& operator=(const ImguiDisabled&) = delete;

private:
    bool _disabled;
};

template <typename E>
bool EnumSlider(ImGuiIf& imgui, const char* label, E* value)
{
    constexpr std::size_t kCount = magic_enum::enum_count<E>();
    static_assert(kCount > 0, "EnumSlider requires non-empty enum");
    int index = static_cast<int>(magic_enum::enum_index(*value).value_or(0));
    std::string preview(magic_enum::enum_name(*value));
    bool changed = imgui.SliderInt(
        label, &index, 0, static_cast<int>(kCount) - 1, preview.c_str());
    if (changed)
    {
        *value = magic_enum::enum_value<E>(static_cast<std::size_t>(index));
    }
    return changed;
}
