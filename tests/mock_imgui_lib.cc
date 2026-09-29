#include "mock_imgui_lib.h"

#include <cstring>

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

ImGuiContext* MockImGuiIf::CreateContext(ImFontAtlas* shared_font_atlas)
{
    (void)shared_font_atlas;
    return nullptr;
}
void MockImGuiIf::DestroyContext(ImGuiContext* ctx)
{
    (void)ctx;
}
ImGuiContext* MockImGuiIf::GetCurrentContext()
{
    return nullptr;
}
void MockImGuiIf::SetCurrentContext(ImGuiContext* ctx)
{
    (void)ctx;
}
ImGuiIO& MockImGuiIf::GetIO()
{
    return _io;
}
ImGuiPlatformIO& MockImGuiIf::GetPlatformIO()
{
    return _platformIO;
}
ImGuiStyle& MockImGuiIf::GetStyle()
{
    return _style;
}
void MockImGuiIf::NewFrame() {}
void MockImGuiIf::EndFrame() {}
void MockImGuiIf::Render() {}
ImDrawData* MockImGuiIf::GetDrawData()
{
    return nullptr;
}
void MockImGuiIf::ShowDemoWindow(bool* p_open)
{
    (void)p_open;
}
void MockImGuiIf::ShowMetricsWindow(bool* p_open)
{
    (void)p_open;
}
void MockImGuiIf::ShowDebugLogWindow(bool* p_open)
{
    (void)p_open;
}
void MockImGuiIf::ShowIDStackToolWindow(bool* p_open)
{
    (void)p_open;
}
void MockImGuiIf::ShowAboutWindow(bool* p_open)
{
    (void)p_open;
}
void MockImGuiIf::ShowStyleEditor(ImGuiStyle* ref)
{
    (void)ref;
}
bool MockImGuiIf::ShowStyleSelector(const char* label)
{
    (void)label;
    return false;
}
void MockImGuiIf::ShowFontSelector(const char* label)
{
    (void)label;
}
void MockImGuiIf::ShowUserGuide() {}
const char* MockImGuiIf::GetVersion()
{
    return nullptr;
}
void MockImGuiIf::StyleColorsDark(ImGuiStyle* dst)
{
    (void)dst;
}
void MockImGuiIf::StyleColorsLight(ImGuiStyle* dst)
{
    (void)dst;
}
void MockImGuiIf::StyleColorsClassic(ImGuiStyle* dst)
{
    (void)dst;
}
bool MockImGuiIf::Begin(const char* name, bool* p_open, ImGuiWindowFlags flags)
{
    (void)name;
    (void)p_open;
    (void)flags;
    return false;
}
void MockImGuiIf::End() {}
bool MockImGuiIf::BeginChild(const char* str_id,
    const ImVec2& size,
    ImGuiChildFlags child_flags,
    ImGuiWindowFlags window_flags)
{
    (void)str_id;
    (void)size;
    (void)child_flags;
    (void)window_flags;
    return false;
}
bool MockImGuiIf::BeginChild(ImGuiID id,
    const ImVec2& size,
    ImGuiChildFlags child_flags,
    ImGuiWindowFlags window_flags)
{
    (void)id;
    (void)size;
    (void)child_flags;
    (void)window_flags;
    return false;
}
void MockImGuiIf::EndChild() {}
bool MockImGuiIf::IsWindowAppearing()
{
    return false;
}
bool MockImGuiIf::IsWindowCollapsed()
{
    return false;
}
bool MockImGuiIf::IsWindowFocused(ImGuiFocusedFlags flags)
{
    (void)flags;
    return false;
}
bool MockImGuiIf::IsWindowHovered(ImGuiHoveredFlags flags)
{
    (void)flags;
    return false;
}
ImDrawList* MockImGuiIf::GetWindowDrawList()
{
    return nullptr;
}
float MockImGuiIf::GetWindowDpiScale()
{
    return 0;
}
ImVec2 MockImGuiIf::GetWindowPos()
{
    return ImVec2(0, 0);
}
ImVec2 MockImGuiIf::GetWindowSize()
{
    return ImVec2(0, 0);
}
float MockImGuiIf::GetWindowWidth()
{
    return 0;
}
float MockImGuiIf::GetWindowHeight()
{
    return 0;
}
ImGuiViewport* MockImGuiIf::GetWindowViewport()
{
    return nullptr;
}
void MockImGuiIf::SetNextWindowPos(
    const ImVec2& pos, ImGuiCond cond, const ImVec2& pivot)
{
    (void)pos;
    (void)cond;
    (void)pivot;
}
void MockImGuiIf::SetNextWindowSize(const ImVec2& size, ImGuiCond cond)
{
    (void)size;
    (void)cond;
}
void MockImGuiIf::SetNextWindowSizeConstraints(const ImVec2& size_min,
    const ImVec2& size_max,
    ImGuiSizeCallback custom_callback,
    void* custom_callback_data)
{
    (void)size_min;
    (void)size_max;
    (void)custom_callback;
    (void)custom_callback_data;
}
void MockImGuiIf::SetNextWindowContentSize(const ImVec2& size)
{
    (void)size;
}
void MockImGuiIf::SetNextWindowCollapsed(bool collapsed, ImGuiCond cond)
{
    (void)collapsed;
    (void)cond;
}
void MockImGuiIf::SetNextWindowFocus() {}
void MockImGuiIf::SetNextWindowScroll(const ImVec2& scroll)
{
    (void)scroll;
}
void MockImGuiIf::SetNextWindowBgAlpha(float alpha)
{
    (void)alpha;
}
void MockImGuiIf::SetNextWindowViewport(ImGuiID viewport_id)
{
    (void)viewport_id;
}
void MockImGuiIf::SetWindowPos(const ImVec2& pos, ImGuiCond cond)
{
    (void)pos;
    (void)cond;
}
void MockImGuiIf::SetWindowSize(const ImVec2& size, ImGuiCond cond)
{
    (void)size;
    (void)cond;
}
void MockImGuiIf::SetWindowCollapsed(bool collapsed, ImGuiCond cond)
{
    (void)collapsed;
    (void)cond;
}
void MockImGuiIf::SetWindowFocus() {}
void MockImGuiIf::SetWindowPos(
    const char* name, const ImVec2& pos, ImGuiCond cond)
{
    (void)name;
    (void)pos;
    (void)cond;
}
void MockImGuiIf::SetWindowSize(
    const char* name, const ImVec2& size, ImGuiCond cond)
{
    (void)name;
    (void)size;
    (void)cond;
}
void MockImGuiIf::SetWindowCollapsed(
    const char* name, bool collapsed, ImGuiCond cond)
{
    (void)name;
    (void)collapsed;
    (void)cond;
}
void MockImGuiIf::SetWindowFocus(const char* name)
{
    (void)name;
}
float MockImGuiIf::GetScrollX()
{
    return 0;
}
float MockImGuiIf::GetScrollY()
{
    return 0;
}
void MockImGuiIf::SetScrollX(float scroll_x)
{
    (void)scroll_x;
}
void MockImGuiIf::SetScrollY(float scroll_y)
{
    (void)scroll_y;
}
float MockImGuiIf::GetScrollMaxX()
{
    return 0;
}
float MockImGuiIf::GetScrollMaxY()
{
    return 0;
}
void MockImGuiIf::SetScrollHereX(float center_x_ratio)
{
    (void)center_x_ratio;
}
void MockImGuiIf::SetScrollHereY(float center_y_ratio)
{
    (void)center_y_ratio;
}
void MockImGuiIf::SetScrollFromPosX(float local_x, float center_x_ratio)
{
    (void)local_x;
    (void)center_x_ratio;
}
void MockImGuiIf::SetScrollFromPosY(float local_y, float center_y_ratio)
{
    (void)local_y;
    (void)center_y_ratio;
}
void MockImGuiIf::PushFont(ImFont* font, float font_size_base_unscaled)
{
    (void)font;
    (void)font_size_base_unscaled;
}
void MockImGuiIf::PopFont() {}
ImFont* MockImGuiIf::GetFont()
{
    return _font;
}
float MockImGuiIf::GetFontSize()
{
    return 13.0f;
}
ImFontBaked* MockImGuiIf::GetFontBaked()
{
    return nullptr;
}
void MockImGuiIf::PushStyleColor(ImGuiCol idx, ImU32 col)
{
    (void)idx;
    (void)col;
}
void MockImGuiIf::PushStyleColor(ImGuiCol idx, const ImVec4& col)
{
    (void)idx;
    (void)col;
}
void MockImGuiIf::PopStyleColor(int count)
{
    (void)count;
}
void MockImGuiIf::PushStyleVar(ImGuiStyleVar idx, float val)
{
    (void)idx;
    (void)val;
}
void MockImGuiIf::PushStyleVar(ImGuiStyleVar idx, const ImVec2& val)
{
    (void)idx;
    (void)val;
}
void MockImGuiIf::PushStyleVarX(ImGuiStyleVar idx, float val_x)
{
    (void)idx;
    (void)val_x;
}
void MockImGuiIf::PushStyleVarY(ImGuiStyleVar idx, float val_y)
{
    (void)idx;
    (void)val_y;
}
void MockImGuiIf::PopStyleVar(int count)
{
    (void)count;
}
void MockImGuiIf::PushItemFlag(ImGuiItemFlags option, bool enabled)
{
    (void)option;
    (void)enabled;
}
void MockImGuiIf::PopItemFlag() {}
void MockImGuiIf::PushItemWidth(float item_width)
{
    (void)item_width;
}
void MockImGuiIf::PopItemWidth() {}
void MockImGuiIf::SetNextItemWidth(float item_width)
{
    (void)item_width;
}
float MockImGuiIf::CalcItemWidth()
{
    return 0;
}
void MockImGuiIf::PushTextWrapPos(float wrap_local_pos_x)
{
    (void)wrap_local_pos_x;
}
void MockImGuiIf::PopTextWrapPos() {}
ImVec2 MockImGuiIf::GetFontTexUvWhitePixel()
{
    return ImVec2(0, 0);
}
ImU32 MockImGuiIf::GetColorU32(ImGuiCol idx, float alpha_mul)
{
    (void)idx;
    (void)alpha_mul;
    return 0;
}
ImU32 MockImGuiIf::GetColorU32(const ImVec4& col)
{
    (void)col;
    return 0;
}
ImU32 MockImGuiIf::GetColorU32(ImU32 col, float alpha_mul)
{
    (void)col;
    (void)alpha_mul;
    return 0;
}
const ImVec4& MockImGuiIf::GetStyleColorVec4(ImGuiCol idx)
{
    (void)idx;
    return _dummyVec4;
}
ImVec2 MockImGuiIf::GetCursorScreenPos()
{
    return ImVec2(0, 0);
}
void MockImGuiIf::SetCursorScreenPos(const ImVec2& pos)
{
    (void)pos;
}
ImVec2 MockImGuiIf::GetContentRegionAvail()
{
    return ImVec2(0, 0);
}
ImVec2 MockImGuiIf::GetCursorPos()
{
    return ImVec2(0, 0);
}
float MockImGuiIf::GetCursorPosX()
{
    return 0;
}
float MockImGuiIf::GetCursorPosY()
{
    return 0;
}
void MockImGuiIf::SetCursorPos(const ImVec2& local_pos)
{
    (void)local_pos;
}
void MockImGuiIf::SetCursorPosX(float local_x)
{
    (void)local_x;
}
void MockImGuiIf::SetCursorPosY(float local_y)
{
    (void)local_y;
}
ImVec2 MockImGuiIf::GetCursorStartPos()
{
    return ImVec2(0, 0);
}
void MockImGuiIf::Separator() {}
void MockImGuiIf::SameLine(float offset_from_start_x, float spacing)
{
    (void)offset_from_start_x;
    (void)spacing;
}
void MockImGuiIf::NewLine() {}
void MockImGuiIf::Spacing() {}
void MockImGuiIf::Dummy(const ImVec2& size)
{
    (void)size;
}
void MockImGuiIf::Indent(float indent_w)
{
    (void)indent_w;
}
void MockImGuiIf::Unindent(float indent_w)
{
    (void)indent_w;
}
void MockImGuiIf::BeginGroup() {}
void MockImGuiIf::EndGroup() {}
void MockImGuiIf::AlignTextToFramePadding() {}
float MockImGuiIf::GetTextLineHeight()
{
    return 0;
}
float MockImGuiIf::GetTextLineHeightWithSpacing()
{
    return 0;
}
float MockImGuiIf::GetFrameHeight()
{
    return 0;
}
float MockImGuiIf::GetFrameHeightWithSpacing()
{
    return 0;
}
void MockImGuiIf::PushID(const char* str_id)
{
    (void)str_id;
}
void MockImGuiIf::PushID(const char* str_id_begin, const char* str_id_end)
{
    (void)str_id_begin;
    (void)str_id_end;
}
void MockImGuiIf::PushID(const void* ptr_id)
{
    (void)ptr_id;
}
void MockImGuiIf::PushID(int int_id)
{
    (void)int_id;
}
void MockImGuiIf::PopID() {}
ImGuiID MockImGuiIf::GetID(const char* str_id)
{
    (void)str_id;
    return 0;
}
ImGuiID MockImGuiIf::GetID(const char* str_id_begin, const char* str_id_end)
{
    (void)str_id_begin;
    (void)str_id_end;
    return 0;
}
ImGuiID MockImGuiIf::GetID(const void* ptr_id)
{
    (void)ptr_id;
    return 0;
}
ImGuiID MockImGuiIf::GetID(int int_id)
{
    (void)int_id;
    return 0;
}
void MockImGuiIf::TextUnformatted(const char* text, const char* text_end)
{
    (void)text;
    (void)text_end;
}
void MockImGuiIf::Text(const char* fmt, ...)
{
    (void)fmt;
}
void MockImGuiIf::TextV(const char* fmt, va_list args)
{
    (void)fmt;
    (void)args;
}
void MockImGuiIf::TextColored(const ImVec4& col, const char* fmt, ...)
{
    (void)col;
    (void)fmt;
}
void MockImGuiIf::TextColoredV(const ImVec4& col, const char* fmt, va_list args)
{
    (void)col;
    (void)fmt;
    (void)args;
}
void MockImGuiIf::TextDisabled(const char* fmt, ...)
{
    (void)fmt;
}
void MockImGuiIf::TextDisabledV(const char* fmt, va_list args)
{
    (void)fmt;
    (void)args;
}
void MockImGuiIf::TextWrapped(const char* fmt, ...)
{
    (void)fmt;
}
void MockImGuiIf::TextWrappedV(const char* fmt, va_list args)
{
    (void)fmt;
    (void)args;
}
void MockImGuiIf::LabelText(const char* label, const char* fmt, ...)
{
    (void)label;
    (void)fmt;
}
void MockImGuiIf::LabelTextV(const char* label, const char* fmt, va_list args)
{
    (void)label;
    (void)fmt;
    (void)args;
}
void MockImGuiIf::BulletText(const char* fmt, ...)
{
    (void)fmt;
}
void MockImGuiIf::BulletTextV(const char* fmt, va_list args)
{
    (void)fmt;
    (void)args;
}
void MockImGuiIf::SeparatorText(const char* label)
{
    (void)label;
}
bool MockImGuiIf::Button(const char* label, const ImVec2& size)
{
    (void)label;
    (void)size;
    return false;
}
bool MockImGuiIf::SmallButton(const char* label)
{
    (void)label;
    return false;
}
bool MockImGuiIf::InvisibleButton(
    const char* str_id, const ImVec2& size, ImGuiButtonFlags flags)
{
    (void)str_id;
    (void)size;
    (void)flags;
    return false;
}
bool MockImGuiIf::ArrowButton(const char* str_id, ImGuiDir dir)
{
    (void)str_id;
    (void)dir;
    return false;
}
bool MockImGuiIf::Checkbox(const char* label, bool* v)
{
    (void)label;
    (void)v;
    return false;
}
bool MockImGuiIf::CheckboxFlags(const char* label, int* flags, int flags_value)
{
    (void)label;
    (void)flags;
    (void)flags_value;
    return false;
}
bool MockImGuiIf::CheckboxFlags(
    const char* label, unsigned int* flags, unsigned int flags_value)
{
    (void)label;
    (void)flags;
    (void)flags_value;
    return false;
}
bool MockImGuiIf::RadioButton(const char* label, bool active)
{
    (void)label;
    (void)active;
    return false;
}
bool MockImGuiIf::RadioButton(const char* label, int* v, int v_button)
{
    (void)label;
    (void)v;
    (void)v_button;
    return false;
}
void MockImGuiIf::ProgressBar(
    float fraction, const ImVec2& size_arg, const char* overlay)
{
    (void)fraction;
    (void)size_arg;
    (void)overlay;
}
void MockImGuiIf::Bullet() {}
bool MockImGuiIf::TextLink(const char* label)
{
    (void)label;
    return false;
}
bool MockImGuiIf::TextLinkOpenURL(const char* label, const char* url)
{
    (void)label;
    (void)url;
    return false;
}
void MockImGuiIf::Image(ImTextureRef tex_ref,
    const ImVec2& image_size,
    const ImVec2& uv0,
    const ImVec2& uv1)
{
    (void)tex_ref;
    (void)image_size;
    (void)uv0;
    (void)uv1;
}
void MockImGuiIf::ImageWithBg(ImTextureRef tex_ref,
    const ImVec2& image_size,
    const ImVec2& uv0,
    const ImVec2& uv1,
    const ImVec4& bg_col,
    const ImVec4& tint_col)
{
    (void)tex_ref;
    (void)image_size;
    (void)uv0;
    (void)uv1;
    (void)bg_col;
    (void)tint_col;
}
bool MockImGuiIf::ImageButton(const char* str_id,
    ImTextureRef tex_ref,
    const ImVec2& image_size,
    const ImVec2& uv0,
    const ImVec2& uv1,
    const ImVec4& bg_col,
    const ImVec4& tint_col)
{
    (void)str_id;
    (void)tex_ref;
    (void)image_size;
    (void)uv0;
    (void)uv1;
    (void)bg_col;
    (void)tint_col;
    return false;
}
bool MockImGuiIf::BeginCombo(
    const char* label, const char* preview_value, ImGuiComboFlags flags)
{
    (void)label;
    (void)preview_value;
    (void)flags;
    return false;
}
void MockImGuiIf::EndCombo() {}
bool MockImGuiIf::Combo(const char* label,
    int* current_item,
    const char* const items[],
    int items_count,
    int popup_max_height_in_items)
{
    (void)label;
    (void)current_item;
    (void)items;
    (void)items_count;
    (void)popup_max_height_in_items;
    return false;
}
bool MockImGuiIf::Combo(const char* label,
    int* current_item,
    const char* items_separated_by_zeros,
    int popup_max_height_in_items)
{
    (void)label;
    (void)current_item;
    (void)items_separated_by_zeros;
    (void)popup_max_height_in_items;
    return false;
}
bool MockImGuiIf::Combo(const char* label,
    int* current_item,
    const char* (*getter)(void* user_data, int idx),
    void* user_data,
    int items_count,
    int popup_max_height_in_items)
{
    (void)label;
    (void)current_item;
    (void)getter;
    (void)user_data;
    (void)items_count;
    (void)popup_max_height_in_items;
    return false;
}
bool MockImGuiIf::DragFloat(const char* label,
    float* v,
    float v_speed,
    float v_min,
    float v_max,
    const char* format,
    ImGuiSliderFlags flags)
{
    (void)label;
    (void)v;
    (void)v_speed;
    (void)v_min;
    (void)v_max;
    (void)format;
    (void)flags;
    return false;
}
bool MockImGuiIf::DragFloat2(const char* label,
    float v[2],
    float v_speed,
    float v_min,
    float v_max,
    const char* format,
    ImGuiSliderFlags flags)
{
    (void)label;
    (void)v;
    (void)v_speed;
    (void)v_min;
    (void)v_max;
    (void)format;
    (void)flags;
    return false;
}
bool MockImGuiIf::DragFloat3(const char* label,
    float v[3],
    float v_speed,
    float v_min,
    float v_max,
    const char* format,
    ImGuiSliderFlags flags)
{
    (void)label;
    (void)v;
    (void)v_speed;
    (void)v_min;
    (void)v_max;
    (void)format;
    (void)flags;
    return false;
}
bool MockImGuiIf::DragFloat4(const char* label,
    float v[4],
    float v_speed,
    float v_min,
    float v_max,
    const char* format,
    ImGuiSliderFlags flags)
{
    (void)label;
    (void)v;
    (void)v_speed;
    (void)v_min;
    (void)v_max;
    (void)format;
    (void)flags;
    return false;
}
bool MockImGuiIf::DragFloatRange2(const char* label,
    float* v_current_min,
    float* v_current_max,
    float v_speed,
    float v_min,
    float v_max,
    const char* format,
    const char* format_max,
    ImGuiSliderFlags flags)
{
    (void)label;
    (void)v_current_min;
    (void)v_current_max;
    (void)v_speed;
    (void)v_min;
    (void)v_max;
    (void)format;
    (void)format_max;
    (void)flags;
    return false;
}
bool MockImGuiIf::DragInt(const char* label,
    int* v,
    float v_speed,
    int v_min,
    int v_max,
    const char* format,
    ImGuiSliderFlags flags)
{
    (void)label;
    (void)v;
    (void)v_speed;
    (void)v_min;
    (void)v_max;
    (void)format;
    (void)flags;
    return false;
}
bool MockImGuiIf::DragInt2(const char* label,
    int v[2],
    float v_speed,
    int v_min,
    int v_max,
    const char* format,
    ImGuiSliderFlags flags)
{
    (void)label;
    (void)v;
    (void)v_speed;
    (void)v_min;
    (void)v_max;
    (void)format;
    (void)flags;
    return false;
}
bool MockImGuiIf::DragInt3(const char* label,
    int v[3],
    float v_speed,
    int v_min,
    int v_max,
    const char* format,
    ImGuiSliderFlags flags)
{
    (void)label;
    (void)v;
    (void)v_speed;
    (void)v_min;
    (void)v_max;
    (void)format;
    (void)flags;
    return false;
}
bool MockImGuiIf::DragInt4(const char* label,
    int v[4],
    float v_speed,
    int v_min,
    int v_max,
    const char* format,
    ImGuiSliderFlags flags)
{
    (void)label;
    (void)v;
    (void)v_speed;
    (void)v_min;
    (void)v_max;
    (void)format;
    (void)flags;
    return false;
}
bool MockImGuiIf::DragIntRange2(const char* label,
    int* v_current_min,
    int* v_current_max,
    float v_speed,
    int v_min,
    int v_max,
    const char* format,
    const char* format_max,
    ImGuiSliderFlags flags)
{
    (void)label;
    (void)v_current_min;
    (void)v_current_max;
    (void)v_speed;
    (void)v_min;
    (void)v_max;
    (void)format;
    (void)format_max;
    (void)flags;
    return false;
}
bool MockImGuiIf::DragScalar(const char* label,
    ImGuiDataType data_type,
    void* p_data,
    float v_speed,
    const void* p_min,
    const void* p_max,
    const char* format,
    ImGuiSliderFlags flags)
{
    (void)label;
    (void)data_type;
    (void)p_data;
    (void)v_speed;
    (void)p_min;
    (void)p_max;
    (void)format;
    (void)flags;
    return false;
}
bool MockImGuiIf::DragScalarN(const char* label,
    ImGuiDataType data_type,
    void* p_data,
    int components,
    float v_speed,
    const void* p_min,
    const void* p_max,
    const char* format,
    ImGuiSliderFlags flags)
{
    (void)label;
    (void)data_type;
    (void)p_data;
    (void)components;
    (void)v_speed;
    (void)p_min;
    (void)p_max;
    (void)format;
    (void)flags;
    return false;
}
bool MockImGuiIf::SliderFloat(const char* label,
    float* v,
    float v_min,
    float v_max,
    const char* format,
    ImGuiSliderFlags flags)
{
    (void)label;
    (void)v;
    (void)v_min;
    (void)v_max;
    (void)format;
    (void)flags;
    return false;
}
bool MockImGuiIf::SliderFloat2(const char* label,
    float v[2],
    float v_min,
    float v_max,
    const char* format,
    ImGuiSliderFlags flags)
{
    (void)label;
    (void)v;
    (void)v_min;
    (void)v_max;
    (void)format;
    (void)flags;
    return false;
}
bool MockImGuiIf::SliderFloat3(const char* label,
    float v[3],
    float v_min,
    float v_max,
    const char* format,
    ImGuiSliderFlags flags)
{
    (void)label;
    (void)v;
    (void)v_min;
    (void)v_max;
    (void)format;
    (void)flags;
    return false;
}
bool MockImGuiIf::SliderFloat4(const char* label,
    float v[4],
    float v_min,
    float v_max,
    const char* format,
    ImGuiSliderFlags flags)
{
    (void)label;
    (void)v;
    (void)v_min;
    (void)v_max;
    (void)format;
    (void)flags;
    return false;
}
bool MockImGuiIf::SliderAngle(const char* label,
    float* v_rad,
    float v_degrees_min,
    float v_degrees_max,
    const char* format,
    ImGuiSliderFlags flags)
{
    (void)label;
    (void)v_rad;
    (void)v_degrees_min;
    (void)v_degrees_max;
    (void)format;
    (void)flags;
    return false;
}
bool MockImGuiIf::SliderInt(const char* label,
    int* v,
    int v_min,
    int v_max,
    const char* format,
    ImGuiSliderFlags flags)
{
    (void)label;
    (void)v;
    (void)v_min;
    (void)v_max;
    (void)format;
    (void)flags;
    return false;
}
bool MockImGuiIf::SliderInt2(const char* label,
    int v[2],
    int v_min,
    int v_max,
    const char* format,
    ImGuiSliderFlags flags)
{
    (void)label;
    (void)v;
    (void)v_min;
    (void)v_max;
    (void)format;
    (void)flags;
    return false;
}
bool MockImGuiIf::SliderInt3(const char* label,
    int v[3],
    int v_min,
    int v_max,
    const char* format,
    ImGuiSliderFlags flags)
{
    (void)label;
    (void)v;
    (void)v_min;
    (void)v_max;
    (void)format;
    (void)flags;
    return false;
}
bool MockImGuiIf::SliderInt4(const char* label,
    int v[4],
    int v_min,
    int v_max,
    const char* format,
    ImGuiSliderFlags flags)
{
    (void)label;
    (void)v;
    (void)v_min;
    (void)v_max;
    (void)format;
    (void)flags;
    return false;
}
bool MockImGuiIf::SliderScalar(const char* label,
    ImGuiDataType data_type,
    void* p_data,
    const void* p_min,
    const void* p_max,
    const char* format,
    ImGuiSliderFlags flags)
{
    (void)label;
    (void)data_type;
    (void)p_data;
    (void)p_min;
    (void)p_max;
    (void)format;
    (void)flags;
    return false;
}
bool MockImGuiIf::SliderScalarN(const char* label,
    ImGuiDataType data_type,
    void* p_data,
    int components,
    const void* p_min,
    const void* p_max,
    const char* format,
    ImGuiSliderFlags flags)
{
    (void)label;
    (void)data_type;
    (void)p_data;
    (void)components;
    (void)p_min;
    (void)p_max;
    (void)format;
    (void)flags;
    return false;
}
bool MockImGuiIf::VSliderFloat(const char* label,
    const ImVec2& size,
    float* v,
    float v_min,
    float v_max,
    const char* format,
    ImGuiSliderFlags flags)
{
    (void)label;
    (void)size;
    (void)v;
    (void)v_min;
    (void)v_max;
    (void)format;
    (void)flags;
    return false;
}
bool MockImGuiIf::VSliderInt(const char* label,
    const ImVec2& size,
    int* v,
    int v_min,
    int v_max,
    const char* format,
    ImGuiSliderFlags flags)
{
    (void)label;
    (void)size;
    (void)v;
    (void)v_min;
    (void)v_max;
    (void)format;
    (void)flags;
    return false;
}
bool MockImGuiIf::VSliderScalar(const char* label,
    const ImVec2& size,
    ImGuiDataType data_type,
    void* p_data,
    const void* p_min,
    const void* p_max,
    const char* format,
    ImGuiSliderFlags flags)
{
    (void)label;
    (void)size;
    (void)data_type;
    (void)p_data;
    (void)p_min;
    (void)p_max;
    (void)format;
    (void)flags;
    return false;
}
bool MockImGuiIf::InputText(const char* label,
    char* buf,
    size_t buf_size,
    ImGuiInputTextFlags flags,
    ImGuiInputTextCallback callback,
    void* user_data)
{
    (void)label;
    (void)buf;
    (void)buf_size;
    (void)flags;
    (void)callback;
    (void)user_data;
    return false;
}
bool MockImGuiIf::InputTextMultiline(const char* label,
    char* buf,
    size_t buf_size,
    const ImVec2& size,
    ImGuiInputTextFlags flags,
    ImGuiInputTextCallback callback,
    void* user_data)
{
    (void)label;
    (void)buf;
    (void)buf_size;
    (void)size;
    (void)flags;
    (void)callback;
    (void)user_data;
    return false;
}
bool MockImGuiIf::InputTextWithHint(const char* label,
    const char* hint,
    char* buf,
    size_t buf_size,
    ImGuiInputTextFlags flags,
    ImGuiInputTextCallback callback,
    void* user_data)
{
    (void)label;
    (void)hint;
    (void)buf;
    (void)buf_size;
    (void)flags;
    (void)callback;
    (void)user_data;
    return false;
}
bool MockImGuiIf::InputFloat(const char* label,
    float* v,
    float step,
    float step_fast,
    const char* format,
    ImGuiInputTextFlags flags)
{
    (void)label;
    (void)v;
    (void)step;
    (void)step_fast;
    (void)format;
    (void)flags;
    return false;
}
bool MockImGuiIf::InputFloat2(const char* label,
    float v[2],
    const char* format,
    ImGuiInputTextFlags flags)
{
    (void)label;
    (void)v;
    (void)format;
    (void)flags;
    return false;
}
bool MockImGuiIf::InputFloat3(const char* label,
    float v[3],
    const char* format,
    ImGuiInputTextFlags flags)
{
    (void)label;
    (void)v;
    (void)format;
    (void)flags;
    return false;
}
bool MockImGuiIf::InputFloat4(const char* label,
    float v[4],
    const char* format,
    ImGuiInputTextFlags flags)
{
    (void)label;
    (void)v;
    (void)format;
    (void)flags;
    return false;
}
bool MockImGuiIf::InputInt(const char* label,
    int* v,
    int step,
    int step_fast,
    ImGuiInputTextFlags flags)
{
    (void)label;
    (void)v;
    (void)step;
    (void)step_fast;
    (void)flags;
    return false;
}
bool MockImGuiIf::InputInt2(
    const char* label, int v[2], ImGuiInputTextFlags flags)
{
    (void)label;
    (void)v;
    (void)flags;
    return false;
}
bool MockImGuiIf::InputInt3(
    const char* label, int v[3], ImGuiInputTextFlags flags)
{
    (void)label;
    (void)v;
    (void)flags;
    return false;
}
bool MockImGuiIf::InputInt4(
    const char* label, int v[4], ImGuiInputTextFlags flags)
{
    (void)label;
    (void)v;
    (void)flags;
    return false;
}
bool MockImGuiIf::InputDouble(const char* label,
    double* v,
    double step,
    double step_fast,
    const char* format,
    ImGuiInputTextFlags flags)
{
    (void)label;
    (void)v;
    (void)step;
    (void)step_fast;
    (void)format;
    (void)flags;
    return false;
}
bool MockImGuiIf::InputScalar(const char* label,
    ImGuiDataType data_type,
    void* p_data,
    const void* p_step,
    const void* p_step_fast,
    const char* format,
    ImGuiInputTextFlags flags)
{
    (void)label;
    (void)data_type;
    (void)p_data;
    (void)p_step;
    (void)p_step_fast;
    (void)format;
    (void)flags;
    return false;
}
bool MockImGuiIf::InputScalarN(const char* label,
    ImGuiDataType data_type,
    void* p_data,
    int components,
    const void* p_step,
    const void* p_step_fast,
    const char* format,
    ImGuiInputTextFlags flags)
{
    (void)label;
    (void)data_type;
    (void)p_data;
    (void)components;
    (void)p_step;
    (void)p_step_fast;
    (void)format;
    (void)flags;
    return false;
}
bool MockImGuiIf::ColorEdit3(
    const char* label, float col[3], ImGuiColorEditFlags flags)
{
    (void)label;
    (void)col;
    (void)flags;
    return false;
}
bool MockImGuiIf::ColorEdit4(
    const char* label, float col[4], ImGuiColorEditFlags flags)
{
    (void)label;
    (void)col;
    (void)flags;
    return false;
}
bool MockImGuiIf::ColorPicker3(
    const char* label, float col[3], ImGuiColorEditFlags flags)
{
    (void)label;
    (void)col;
    (void)flags;
    return false;
}
bool MockImGuiIf::ColorPicker4(const char* label,
    float col[4],
    ImGuiColorEditFlags flags,
    const float* ref_col)
{
    (void)label;
    (void)col;
    (void)flags;
    (void)ref_col;
    return false;
}
bool MockImGuiIf::ColorButton(const char* desc_id,
    const ImVec4& col,
    ImGuiColorEditFlags flags,
    const ImVec2& size)
{
    (void)desc_id;
    (void)col;
    (void)flags;
    (void)size;
    return false;
}
bool MockImGuiIf::TreeNode(const char* label)
{
    (void)label;
    return false;
}
bool MockImGuiIf::TreeNode(const char* str_id, const char* fmt, ...)
{
    (void)str_id;
    (void)fmt;
    return false;
}
bool MockImGuiIf::TreeNode(const void* ptr_id, const char* fmt, ...)
{
    (void)ptr_id;
    (void)fmt;
    return false;
}
bool MockImGuiIf::TreeNodeV(const char* str_id, const char* fmt, va_list args)
{
    (void)str_id;
    (void)fmt;
    (void)args;
    return false;
}
bool MockImGuiIf::TreeNodeV(const void* ptr_id, const char* fmt, va_list args)
{
    (void)ptr_id;
    (void)fmt;
    (void)args;
    return false;
}
bool MockImGuiIf::TreeNodeEx(const char* label, ImGuiTreeNodeFlags flags)
{
    (void)label;
    (void)flags;
    return false;
}
bool MockImGuiIf::TreeNodeEx(
    const char* str_id, ImGuiTreeNodeFlags flags, const char* fmt, ...)
{
    (void)str_id;
    (void)flags;
    (void)fmt;
    return false;
}
bool MockImGuiIf::TreeNodeEx(
    const void* ptr_id, ImGuiTreeNodeFlags flags, const char* fmt, ...)
{
    (void)ptr_id;
    (void)flags;
    (void)fmt;
    return false;
}
bool MockImGuiIf::TreeNodeExV(
    const char* str_id, ImGuiTreeNodeFlags flags, const char* fmt, va_list args)
{
    (void)str_id;
    (void)flags;
    (void)fmt;
    (void)args;
    return false;
}
bool MockImGuiIf::TreeNodeExV(
    const void* ptr_id, ImGuiTreeNodeFlags flags, const char* fmt, va_list args)
{
    (void)ptr_id;
    (void)flags;
    (void)fmt;
    (void)args;
    return false;
}
void MockImGuiIf::TreePush(const char* str_id)
{
    (void)str_id;
}
void MockImGuiIf::TreePush(const void* ptr_id)
{
    (void)ptr_id;
}
void MockImGuiIf::TreePop() {}
float MockImGuiIf::GetTreeNodeToLabelSpacing()
{
    return 0;
}
bool MockImGuiIf::CollapsingHeader(const char* label, ImGuiTreeNodeFlags flags)
{
    (void)label;
    (void)flags;
    return false;
}
bool MockImGuiIf::CollapsingHeader(
    const char* label, bool* p_visible, ImGuiTreeNodeFlags flags)
{
    (void)label;
    (void)p_visible;
    (void)flags;
    return false;
}
void MockImGuiIf::SetNextItemOpen(bool is_open, ImGuiCond cond)
{
    (void)is_open;
    (void)cond;
}
void MockImGuiIf::SetNextItemStorageID(ImGuiID storage_id)
{
    (void)storage_id;
}
bool MockImGuiIf::TreeNodeGetOpen(ImGuiID storage_id)
{
    (void)storage_id;
    return false;
}
bool MockImGuiIf::Selectable(const char* label,
    bool selected,
    ImGuiSelectableFlags flags,
    const ImVec2& size)
{
    (void)label;
    (void)selected;
    (void)flags;
    (void)size;
    return false;
}
bool MockImGuiIf::Selectable(const char* label,
    bool* p_selected,
    ImGuiSelectableFlags flags,
    const ImVec2& size)
{
    (void)label;
    (void)p_selected;
    (void)flags;
    (void)size;
    return false;
}
ImGuiMultiSelectIO* MockImGuiIf::BeginMultiSelect(
    ImGuiMultiSelectFlags flags, int selection_size, int items_count)
{
    (void)flags;
    (void)selection_size;
    (void)items_count;
    return nullptr;
}
ImGuiMultiSelectIO* MockImGuiIf::EndMultiSelect()
{
    return nullptr;
}
void MockImGuiIf::SetNextItemSelectionUserData(
    ImGuiSelectionUserData selection_user_data)
{
    (void)selection_user_data;
}
bool MockImGuiIf::IsItemToggledSelection()
{
    return false;
}
bool MockImGuiIf::BeginListBox(const char* label, const ImVec2& size)
{
    (void)label;
    (void)size;
    return false;
}
void MockImGuiIf::EndListBox() {}
bool MockImGuiIf::ListBox(const char* label,
    int* current_item,
    const char* const items[],
    int items_count,
    int height_in_items)
{
    (void)label;
    (void)current_item;
    (void)items;
    (void)items_count;
    (void)height_in_items;
    return false;
}
bool MockImGuiIf::ListBox(const char* label,
    int* current_item,
    const char* (*getter)(void* user_data, int idx),
    void* user_data,
    int items_count,
    int height_in_items)
{
    (void)label;
    (void)current_item;
    (void)getter;
    (void)user_data;
    (void)items_count;
    (void)height_in_items;
    return false;
}
void MockImGuiIf::PlotLines(const char* label,
    const float* values,
    int values_count,
    int values_offset,
    const char* overlay_text,
    float scale_min,
    float scale_max,
    ImVec2 graph_size,
    int stride)
{
    (void)label;
    (void)values;
    (void)values_count;
    (void)values_offset;
    (void)overlay_text;
    (void)scale_min;
    (void)scale_max;
    (void)graph_size;
    (void)stride;
}
void MockImGuiIf::PlotLines(const char* label,
    float (*values_getter)(void* data, int idx),
    void* data,
    int values_count,
    int values_offset,
    const char* overlay_text,
    float scale_min,
    float scale_max,
    ImVec2 graph_size)
{
    (void)label;
    (void)values_getter;
    (void)data;
    (void)values_count;
    (void)values_offset;
    (void)overlay_text;
    (void)scale_min;
    (void)scale_max;
    (void)graph_size;
}
void MockImGuiIf::PlotHistogram(const char* label,
    const float* values,
    int values_count,
    int values_offset,
    const char* overlay_text,
    float scale_min,
    float scale_max,
    ImVec2 graph_size,
    int stride)
{
    (void)label;
    (void)values;
    (void)values_count;
    (void)values_offset;
    (void)overlay_text;
    (void)scale_min;
    (void)scale_max;
    (void)graph_size;
    (void)stride;
}
void MockImGuiIf::PlotHistogram(const char* label,
    float (*values_getter)(void* data, int idx),
    void* data,
    int values_count,
    int values_offset,
    const char* overlay_text,
    float scale_min,
    float scale_max,
    ImVec2 graph_size)
{
    (void)label;
    (void)values_getter;
    (void)data;
    (void)values_count;
    (void)values_offset;
    (void)overlay_text;
    (void)scale_min;
    (void)scale_max;
    (void)graph_size;
}
void MockImGuiIf::Value(const char* prefix, bool b)
{
    (void)prefix;
    (void)b;
}
void MockImGuiIf::Value(const char* prefix, int v)
{
    (void)prefix;
    (void)v;
}
void MockImGuiIf::Value(const char* prefix, unsigned int v)
{
    (void)prefix;
    (void)v;
}
void MockImGuiIf::Value(const char* prefix, float v, const char* float_format)
{
    (void)prefix;
    (void)v;
    (void)float_format;
}
bool MockImGuiIf::BeginMenuBar()
{
    return false;
}
void MockImGuiIf::EndMenuBar() {}
bool MockImGuiIf::BeginMainMenuBar()
{
    return false;
}
void MockImGuiIf::EndMainMenuBar() {}
bool MockImGuiIf::BeginMenu(const char* label, bool enabled)
{
    (void)label;
    (void)enabled;
    return false;
}
void MockImGuiIf::EndMenu() {}
bool MockImGuiIf::MenuItem(
    const char* label, const char* shortcut, bool selected, bool enabled)
{
    (void)label;
    (void)shortcut;
    (void)selected;
    (void)enabled;
    return false;
}
bool MockImGuiIf::MenuItem(
    const char* label, const char* shortcut, bool* p_selected, bool enabled)
{
    (void)label;
    (void)shortcut;
    (void)p_selected;
    (void)enabled;
    return false;
}
bool MockImGuiIf::BeginTooltip()
{
    return false;
}
void MockImGuiIf::EndTooltip() {}
void MockImGuiIf::SetTooltip(const char* fmt, ...)
{
    (void)fmt;
}
void MockImGuiIf::SetTooltipV(const char* fmt, va_list args)
{
    (void)fmt;
    (void)args;
}
bool MockImGuiIf::BeginItemTooltip()
{
    return false;
}
void MockImGuiIf::SetItemTooltip(const char* fmt, ...)
{
    (void)fmt;
}
void MockImGuiIf::SetItemTooltipV(const char* fmt, va_list args)
{
    (void)fmt;
    (void)args;
}
bool MockImGuiIf::BeginPopup(const char* str_id, ImGuiWindowFlags flags)
{
    (void)str_id;
    (void)flags;
    return false;
}
bool MockImGuiIf::BeginPopupModal(
    const char* name, bool* p_open, ImGuiWindowFlags flags)
{
    (void)name;
    (void)p_open;
    (void)flags;
    return false;
}
void MockImGuiIf::EndPopup() {}
bool MockImGuiIf::OpenPopup(const char* str_id, ImGuiPopupFlags popup_flags)
{
    (void)str_id;
    (void)popup_flags;
    return false;
}
bool MockImGuiIf::OpenPopup(ImGuiID id, ImGuiPopupFlags popup_flags)
{
    (void)id;
    (void)popup_flags;
    return false;
}
bool MockImGuiIf::OpenPopupOnItemClick(
    const char* str_id, ImGuiPopupFlags popup_flags)
{
    (void)str_id;
    (void)popup_flags;
    return false;
}
void MockImGuiIf::CloseCurrentPopup() {}
bool MockImGuiIf::BeginPopupContextItem(
    const char* str_id, ImGuiPopupFlags popup_flags)
{
    (void)str_id;
    (void)popup_flags;
    return false;
}
bool MockImGuiIf::BeginPopupContextWindow(
    const char* str_id, ImGuiPopupFlags popup_flags)
{
    (void)str_id;
    (void)popup_flags;
    return false;
}
bool MockImGuiIf::BeginPopupContextVoid(
    const char* str_id, ImGuiPopupFlags popup_flags)
{
    (void)str_id;
    (void)popup_flags;
    return false;
}
bool MockImGuiIf::IsPopupOpen(const char* str_id, ImGuiPopupFlags flags)
{
    (void)str_id;
    (void)flags;
    return false;
}
bool MockImGuiIf::BeginTable(const char* str_id,
    int columns,
    ImGuiTableFlags flags,
    const ImVec2& outer_size,
    float inner_width)
{
    (void)str_id;
    (void)columns;
    (void)flags;
    (void)outer_size;
    (void)inner_width;
    return false;
}
void MockImGuiIf::EndTable() {}
void MockImGuiIf::TableNextRow(
    ImGuiTableRowFlags row_flags, float min_row_height)
{
    (void)row_flags;
    (void)min_row_height;
}
bool MockImGuiIf::TableNextColumn()
{
    return false;
}
bool MockImGuiIf::TableSetColumnIndex(int column_n)
{
    (void)column_n;
    return false;
}
void MockImGuiIf::TableSetupColumn(const char* label,
    ImGuiTableColumnFlags flags,
    float init_width_or_weight,
    ImGuiID user_data)
{
    (void)label;
    (void)flags;
    (void)init_width_or_weight;
    (void)user_data;
}
void MockImGuiIf::TableSetupScrollFreeze(int cols, int rows)
{
    (void)cols;
    (void)rows;
}
void MockImGuiIf::TableHeader(const char* label)
{
    (void)label;
}
void MockImGuiIf::TableHeadersRow() {}
void MockImGuiIf::TableAngledHeadersRow() {}
ImGuiTableSortSpecs* MockImGuiIf::TableGetSortSpecs()
{
    return nullptr;
}
int MockImGuiIf::TableGetColumnCount()
{
    return 0;
}
int MockImGuiIf::TableGetColumnIndex()
{
    return 0;
}
int MockImGuiIf::TableGetRowIndex()
{
    return 0;
}
const char* MockImGuiIf::TableGetColumnName(int column_n)
{
    (void)column_n;
    return nullptr;
}
ImGuiTableColumnFlags MockImGuiIf::TableGetColumnFlags(int column_n)
{
    (void)column_n;
    return 0;
}
void MockImGuiIf::TableSetColumnEnabled(int column_n, bool v)
{
    (void)column_n;
    (void)v;
}
int MockImGuiIf::TableGetHoveredColumn()
{
    return 0;
}
void MockImGuiIf::TableSetBgColor(
    ImGuiTableBgTarget target, ImU32 color, int column_n)
{
    (void)target;
    (void)color;
    (void)column_n;
}
void MockImGuiIf::Columns(int count, const char* id, bool borders)
{
    (void)count;
    (void)id;
    (void)borders;
}
void MockImGuiIf::NextColumn() {}
int MockImGuiIf::GetColumnIndex()
{
    return 0;
}
float MockImGuiIf::GetColumnWidth(int column_index)
{
    (void)column_index;
    return 0;
}
void MockImGuiIf::SetColumnWidth(int column_index, float width)
{
    (void)column_index;
    (void)width;
}
float MockImGuiIf::GetColumnOffset(int column_index)
{
    (void)column_index;
    return 0;
}
void MockImGuiIf::SetColumnOffset(int column_index, float offset_x)
{
    (void)column_index;
    (void)offset_x;
}
int MockImGuiIf::GetColumnsCount()
{
    return 0;
}
bool MockImGuiIf::BeginTabBar(const char* str_id, ImGuiTabBarFlags flags)
{
    (void)str_id;
    (void)flags;
    return false;
}
void MockImGuiIf::EndTabBar() {}
bool MockImGuiIf::BeginTabItem(
    const char* label, bool* p_open, ImGuiTabItemFlags flags)
{
    (void)label;
    (void)p_open;
    (void)flags;
    return false;
}
void MockImGuiIf::EndTabItem() {}
bool MockImGuiIf::TabItemButton(const char* label, ImGuiTabItemFlags flags)
{
    (void)label;
    (void)flags;
    return false;
}
void MockImGuiIf::SetTabItemClosed(const char* tab_or_docked_window_label)
{
    (void)tab_or_docked_window_label;
}
ImGuiID MockImGuiIf::DockSpace(ImGuiID dockspace_id,
    const ImVec2& size,
    ImGuiDockNodeFlags flags,
    const ImGuiWindowClass* window_class)
{
    (void)dockspace_id;
    (void)size;
    (void)flags;
    (void)window_class;
    return 0;
}
ImGuiID MockImGuiIf::DockSpaceOverViewport(ImGuiID dockspace_id,
    const ImGuiViewport* viewport,
    ImGuiDockNodeFlags flags,
    const ImGuiWindowClass* window_class)
{
    (void)dockspace_id;
    (void)viewport;
    (void)flags;
    (void)window_class;
    return 0;
}
void MockImGuiIf::SetNextWindowDockID(ImGuiID dock_id, ImGuiCond cond)
{
    (void)dock_id;
    (void)cond;
}
void MockImGuiIf::SetNextWindowClass(const ImGuiWindowClass* window_class)
{
    (void)window_class;
}
ImGuiID MockImGuiIf::GetWindowDockID()
{
    return 0;
}
bool MockImGuiIf::IsWindowDocked()
{
    return false;
}
void MockImGuiIf::LogToTTY(int auto_open_depth)
{
    (void)auto_open_depth;
}
void MockImGuiIf::LogToFile(int auto_open_depth, const char* filename)
{
    (void)auto_open_depth;
    (void)filename;
}
void MockImGuiIf::LogToClipboard(int auto_open_depth)
{
    (void)auto_open_depth;
}
void MockImGuiIf::LogFinish() {}
void MockImGuiIf::LogButtons() {}
void MockImGuiIf::LogText(const char* fmt, ...)
{
    (void)fmt;
}
void MockImGuiIf::LogTextV(const char* fmt, va_list args)
{
    (void)fmt;
    (void)args;
}
bool MockImGuiIf::BeginDragDropSource(ImGuiDragDropFlags flags)
{
    (void)flags;
    return false;
}
bool MockImGuiIf::SetDragDropPayload(
    const char* type, const void* data, size_t sz, ImGuiCond cond)
{
    (void)type;
    (void)data;
    (void)sz;
    (void)cond;
    return false;
}
void MockImGuiIf::EndDragDropSource() {}
bool MockImGuiIf::BeginDragDropTarget()
{
    return false;
}
const ImGuiPayload* MockImGuiIf::AcceptDragDropPayload(
    const char* type, ImGuiDragDropFlags flags)
{
    (void)type;
    (void)flags;
    return nullptr;
}
void MockImGuiIf::EndDragDropTarget() {}
const ImGuiPayload* MockImGuiIf::GetDragDropPayload()
{
    return nullptr;
}
void MockImGuiIf::BeginDisabled(bool disabled)
{
    (void)disabled;
}
void MockImGuiIf::EndDisabled() {}
void MockImGuiIf::PushClipRect(const ImVec2& clip_rect_min,
    const ImVec2& clip_rect_max,
    bool intersect_with_current_clip_rect)
{
    (void)clip_rect_min;
    (void)clip_rect_max;
    (void)intersect_with_current_clip_rect;
}
void MockImGuiIf::PopClipRect() {}
void MockImGuiIf::SetItemDefaultFocus() {}
void MockImGuiIf::SetKeyboardFocusHere(int offset)
{
    (void)offset;
}
void MockImGuiIf::SetNavCursorVisible(bool visible)
{
    (void)visible;
}
void MockImGuiIf::SetNextItemAllowOverlap() {}
bool MockImGuiIf::IsItemHovered(ImGuiHoveredFlags flags)
{
    (void)flags;
    return false;
}
bool MockImGuiIf::IsItemActive()
{
    return false;
}
bool MockImGuiIf::IsItemFocused()
{
    return false;
}
bool MockImGuiIf::IsItemClicked(ImGuiMouseButton mouse_button)
{
    (void)mouse_button;
    return false;
}
bool MockImGuiIf::IsItemVisible()
{
    return false;
}
bool MockImGuiIf::IsItemEdited()
{
    return false;
}
bool MockImGuiIf::IsItemActivated()
{
    return false;
}
bool MockImGuiIf::IsItemDeactivated()
{
    return false;
}
bool MockImGuiIf::IsItemDeactivatedAfterEdit()
{
    return false;
}
bool MockImGuiIf::IsItemToggledOpen()
{
    return false;
}
bool MockImGuiIf::IsAnyItemHovered()
{
    return false;
}
bool MockImGuiIf::IsAnyItemActive()
{
    return false;
}
bool MockImGuiIf::IsAnyItemFocused()
{
    return false;
}
ImGuiID MockImGuiIf::GetItemID()
{
    return 0;
}
ImVec2 MockImGuiIf::GetItemRectMin()
{
    return ImVec2(0, 0);
}
ImVec2 MockImGuiIf::GetItemRectMax()
{
    return ImVec2(0, 0);
}
ImVec2 MockImGuiIf::GetItemRectSize()
{
    return ImVec2(0, 0);
}
ImGuiItemFlags MockImGuiIf::GetItemFlags()
{
    return 0;
}
int MockImGuiIf::GetItemClickedCountWithSingleClickDelay(
    ImGuiMouseButton mouse_button, float delay)
{
    (void)mouse_button;
    (void)delay;
    return 0;
}
ImGuiViewport* MockImGuiIf::GetMainViewport()
{
    return nullptr;
}
ImDrawList* MockImGuiIf::GetBackgroundDrawList(ImGuiViewport* viewport)
{
    (void)viewport;
    return nullptr;
}
ImDrawList* MockImGuiIf::GetForegroundDrawList(ImGuiViewport* viewport)
{
    (void)viewport;
    return nullptr;
}
bool MockImGuiIf::IsRectVisible(const ImVec2& size)
{
    (void)size;
    return false;
}
bool MockImGuiIf::IsRectVisible(const ImVec2& rect_min, const ImVec2& rect_max)
{
    (void)rect_min;
    (void)rect_max;
    return false;
}
double MockImGuiIf::GetTime()
{
    return 0;
}
int MockImGuiIf::GetFrameCount()
{
    return 0;
}
ImDrawListSharedData* MockImGuiIf::GetDrawListSharedData()
{
    return nullptr;
}
const char* MockImGuiIf::GetStyleColorName(ImGuiCol idx)
{
    (void)idx;
    return nullptr;
}
void MockImGuiIf::SetStateStorage(ImGuiStorage* storage)
{
    (void)storage;
}
ImGuiStorage* MockImGuiIf::GetStateStorage()
{
    return nullptr;
}
ImVec2 MockImGuiIf::CalcTextSize(const char* text,
    const char* text_end,
    bool hide_text_after_double_hash,
    float wrap_width)
{
    (void)text_end;
    (void)hide_text_after_double_hash;
    (void)wrap_width;
    if (!text)
        return ImVec2(0, 0);
    return ImVec2(static_cast<float>(strlen(text)) * 6.0f, 13.0f);
}
ImVec4 MockImGuiIf::ColorConvertU32ToFloat4(ImU32 in)
{
    (void)in;
    return ImVec4(0, 0, 0, 0);
}
ImU32 MockImGuiIf::ColorConvertFloat4ToU32(const ImVec4& in)
{
    (void)in;
    return 0;
}
void MockImGuiIf::ColorConvertRGBtoHSV(
    float r, float g, float b, float& out_h, float& out_s, float& out_v)
{
    (void)r;
    (void)g;
    (void)b;
    (void)out_h;
    (void)out_s;
    (void)out_v;
}
void MockImGuiIf::ColorConvertHSVtoRGB(
    float h, float s, float v, float& out_r, float& out_g, float& out_b)
{
    (void)h;
    (void)s;
    (void)v;
    (void)out_r;
    (void)out_g;
    (void)out_b;
}
bool MockImGuiIf::IsKeyDown(ImGuiKey key)
{
    (void)key;
    return false;
}
bool MockImGuiIf::IsKeyPressed(ImGuiKey key, bool repeat)
{
    (void)key;
    (void)repeat;
    return false;
}
bool MockImGuiIf::IsKeyReleased(ImGuiKey key)
{
    (void)key;
    return false;
}
bool MockImGuiIf::IsKeyChordPressed(ImGuiKeyChord key_chord)
{
    (void)key_chord;
    return false;
}
int MockImGuiIf::GetKeyPressedAmount(
    ImGuiKey key, float repeat_delay, float rate)
{
    (void)key;
    (void)repeat_delay;
    (void)rate;
    return 0;
}
const char* MockImGuiIf::GetKeyName(ImGuiKey key)
{
    (void)key;
    return nullptr;
}
void MockImGuiIf::SetNextFrameWantCaptureKeyboard(bool want_capture_keyboard)
{
    (void)want_capture_keyboard;
}
bool MockImGuiIf::Shortcut(ImGuiKeyChord key_chord, ImGuiInputFlags flags)
{
    (void)key_chord;
    (void)flags;
    return false;
}
void MockImGuiIf::SetNextItemShortcut(
    ImGuiKeyChord key_chord, ImGuiInputFlags flags)
{
    (void)key_chord;
    (void)flags;
}
bool MockImGuiIf::SetItemKeyOwner(ImGuiKey key)
{
    (void)key;
    return false;
}
bool MockImGuiIf::IsMouseDown(ImGuiMouseButton button)
{
    (void)button;
    return false;
}
bool MockImGuiIf::IsMouseClicked(ImGuiMouseButton button, bool repeat)
{
    (void)button;
    (void)repeat;
    return false;
}
bool MockImGuiIf::IsMouseReleased(ImGuiMouseButton button)
{
    (void)button;
    return false;
}
bool MockImGuiIf::IsMouseDoubleClicked(ImGuiMouseButton button)
{
    (void)button;
    return false;
}
bool MockImGuiIf::IsMouseReleasedWithDelay(ImGuiMouseButton button, float delay)
{
    (void)button;
    (void)delay;
    return false;
}
int MockImGuiIf::GetMouseClickedCount(ImGuiMouseButton button)
{
    (void)button;
    return 0;
}
bool MockImGuiIf::IsMouseHoveringRect(
    const ImVec2& r_min, const ImVec2& r_max, bool clip)
{
    (void)r_min;
    (void)r_max;
    (void)clip;
    return false;
}
bool MockImGuiIf::IsMousePosValid(const ImVec2* mouse_pos)
{
    (void)mouse_pos;
    return false;
}
bool MockImGuiIf::IsAnyMouseDown()
{
    return false;
}
ImVec2 MockImGuiIf::GetMousePos()
{
    return ImVec2(0, 0);
}
ImVec2 MockImGuiIf::GetMousePosOnOpeningCurrentPopup()
{
    return ImVec2(0, 0);
}
bool MockImGuiIf::IsMouseDragging(ImGuiMouseButton button, float lock_threshold)
{
    (void)button;
    (void)lock_threshold;
    return false;
}
ImVec2 MockImGuiIf::GetMouseDragDelta(
    ImGuiMouseButton button, float lock_threshold)
{
    (void)button;
    (void)lock_threshold;
    return ImVec2(0, 0);
}
void MockImGuiIf::ResetMouseDragDelta(ImGuiMouseButton button)
{
    (void)button;
}
ImGuiMouseCursor MockImGuiIf::GetMouseCursor()
{
    return 0;
}
void MockImGuiIf::SetMouseCursor(ImGuiMouseCursor cursor_type)
{
    (void)cursor_type;
}
void MockImGuiIf::SetNextFrameWantCaptureMouse(bool want_capture_mouse)
{
    (void)want_capture_mouse;
}
const char* MockImGuiIf::GetClipboardText()
{
    return nullptr;
}
void MockImGuiIf::SetClipboardText(const char* text)
{
    (void)text;
}
void MockImGuiIf::LoadIniSettingsFromDisk(const char* ini_filename)
{
    (void)ini_filename;
}
void MockImGuiIf::LoadIniSettingsFromMemory(
    const char* ini_data, size_t ini_size)
{
    (void)ini_data;
    (void)ini_size;
}
void MockImGuiIf::SaveIniSettingsToDisk(const char* ini_filename)
{
    (void)ini_filename;
}
const char* MockImGuiIf::SaveIniSettingsToMemory(size_t* out_ini_size)
{
    (void)out_ini_size;
    return nullptr;
}
void MockImGuiIf::DebugTextEncoding(const char* text)
{
    (void)text;
}
void MockImGuiIf::DebugFlashStyleColor(ImGuiCol idx)
{
    (void)idx;
}
void MockImGuiIf::DebugStartItemPicker() {}
bool MockImGuiIf::DebugCheckVersionAndDataLayout(const char* version_str,
    size_t sz_io,
    size_t sz_style,
    size_t sz_vec2,
    size_t sz_vec4,
    size_t sz_drawvert,
    size_t sz_drawidx)
{
    (void)version_str;
    (void)sz_io;
    (void)sz_style;
    (void)sz_vec2;
    (void)sz_vec4;
    (void)sz_drawvert;
    (void)sz_drawidx;
    return false;
}
void MockImGuiIf::DebugLog(const char* fmt, ...)
{
    (void)fmt;
}
void MockImGuiIf::DebugLogV(const char* fmt, va_list args)
{
    (void)fmt;
    (void)args;
}
void MockImGuiIf::SetAllocatorFunctions(
    ImGuiMemAllocFunc alloc_func, ImGuiMemFreeFunc free_func, void* user_data)
{
    (void)alloc_func;
    (void)free_func;
    (void)user_data;
}
void MockImGuiIf::GetAllocatorFunctions(ImGuiMemAllocFunc* p_alloc_func,
    ImGuiMemFreeFunc* p_free_func,
    void** p_user_data)
{
    (void)p_alloc_func;
    (void)p_free_func;
    (void)p_user_data;
}
void* MockImGuiIf::MemAlloc(size_t size)
{
    (void)size;
    return nullptr;
}
void MockImGuiIf::MemFree(void* ptr)
{
    (void)ptr;
}
void MockImGuiIf::UpdatePlatformWindows() {}
void MockImGuiIf::RenderPlatformWindowsDefault(
    void* platform_render_arg, void* renderer_render_arg)
{
    (void)platform_render_arg;
    (void)renderer_render_arg;
}
void MockImGuiIf::DestroyPlatformWindows() {}
ImGuiViewport* MockImGuiIf::FindViewportByID(ImGuiID viewport_id)
{
    (void)viewport_id;
    return nullptr;
}
ImGuiViewport* MockImGuiIf::FindViewportByPlatformHandle(void* platform_handle)
{
    (void)platform_handle;
    return nullptr;
}
ImPlotContext* MockImGuiIf::ImPlot_CreateContext()
{
    return nullptr;
}
void MockImGuiIf::ImPlot_DestroyContext(ImPlotContext* ctx)
{
    (void)ctx;
}
ImPlotContext* MockImGuiIf::ImPlot_GetCurrentContext()
{
    return nullptr;
}
void MockImGuiIf::ImPlot_SetCurrentContext(ImPlotContext* ctx)
{
    (void)ctx;
}
void MockImGuiIf::SetImGuiContext(ImGuiContext* ctx)
{
    (void)ctx;
}
bool MockImGuiIf::BeginPlot(
    const char* title_id, const ImVec2& size, ImPlotFlags flags)
{
    (void)title_id;
    (void)size;
    (void)flags;
    return false;
}
void MockImGuiIf::EndPlot() {}
bool MockImGuiIf::BeginSubplots(const char* title_id,
    int rows,
    int cols,
    const ImVec2& size,
    ImPlotSubplotFlags flags,
    float* row_ratios,
    float* col_ratios)
{
    (void)title_id;
    (void)rows;
    (void)cols;
    (void)size;
    (void)flags;
    (void)row_ratios;
    (void)col_ratios;
    return false;
}
void MockImGuiIf::EndSubplots() {}
void MockImGuiIf::SetupAxis(
    ImAxis axis, const char* label, ImPlotAxisFlags flags)
{
    (void)axis;
    (void)label;
    (void)flags;
}
void MockImGuiIf::SetupAxisLimits(
    ImAxis axis, double v_min, double v_max, ImPlotCond cond)
{
    (void)axis;
    (void)v_min;
    (void)v_max;
    (void)cond;
}
void MockImGuiIf::SetupAxisLinks(
    ImAxis axis, double* link_min, double* link_max)
{
    (void)axis;
    (void)link_min;
    (void)link_max;
}
void MockImGuiIf::SetupAxisFormat(ImAxis axis, const char* fmt)
{
    (void)axis;
    (void)fmt;
}
void MockImGuiIf::SetupAxisFormat(
    ImAxis axis, ImPlotFormatter formatter, void* data)
{
    (void)axis;
    (void)formatter;
    (void)data;
}
void MockImGuiIf::SetupAxisTicks(ImAxis axis,
    const double* values,
    int n_ticks,
    const char* const labels[],
    bool keep_default)
{
    (void)axis;
    (void)values;
    (void)n_ticks;
    (void)labels;
    (void)keep_default;
}
void MockImGuiIf::SetupAxisTicks(ImAxis axis,
    double v_min,
    double v_max,
    int n_ticks,
    const char* const labels[],
    bool keep_default)
{
    (void)axis;
    (void)v_min;
    (void)v_max;
    (void)n_ticks;
    (void)labels;
    (void)keep_default;
}
void MockImGuiIf::SetupAxisScale(ImAxis axis, ImPlotScale scale)
{
    (void)axis;
    (void)scale;
}
void MockImGuiIf::SetupAxisScale(
    ImAxis axis, ImPlotTransform forward, ImPlotTransform inverse, void* data)
{
    (void)axis;
    (void)forward;
    (void)inverse;
    (void)data;
}
void MockImGuiIf::SetupAxisLimitsConstraints(
    ImAxis axis, double v_min, double v_max)
{
    (void)axis;
    (void)v_min;
    (void)v_max;
}
void MockImGuiIf::SetupAxisZoomConstraints(
    ImAxis axis, double z_min, double z_max)
{
    (void)axis;
    (void)z_min;
    (void)z_max;
}
void MockImGuiIf::SetupAxes(const char* x_label,
    const char* y_label,
    ImPlotAxisFlags x_flags,
    ImPlotAxisFlags y_flags)
{
    (void)x_label;
    (void)y_label;
    (void)x_flags;
    (void)y_flags;
}
void MockImGuiIf::SetupAxesLimits(
    double x_min, double x_max, double y_min, double y_max, ImPlotCond cond)
{
    (void)x_min;
    (void)x_max;
    (void)y_min;
    (void)y_max;
    (void)cond;
}
void MockImGuiIf::SetupLegend(ImPlotLocation location, ImPlotLegendFlags flags)
{
    (void)location;
    (void)flags;
}
void MockImGuiIf::SetupMouseText(
    ImPlotLocation location, ImPlotMouseTextFlags flags)
{
    (void)location;
    (void)flags;
}
void MockImGuiIf::SetupFinish() {}
void MockImGuiIf::SetNextAxisLimits(
    ImAxis axis, double v_min, double v_max, ImPlotCond cond)
{
    (void)axis;
    (void)v_min;
    (void)v_max;
    (void)cond;
}
void MockImGuiIf::SetNextAxisLinks(
    ImAxis axis, double* link_min, double* link_max)
{
    (void)axis;
    (void)link_min;
    (void)link_max;
}
void MockImGuiIf::SetNextAxisToFit(ImAxis axis)
{
    (void)axis;
}
void MockImGuiIf::SetNextAxesLimits(
    double x_min, double x_max, double y_min, double y_max, ImPlotCond cond)
{
    (void)x_min;
    (void)x_max;
    (void)y_min;
    (void)y_max;
    (void)cond;
}
void MockImGuiIf::SetNextAxesToFit() {}
void MockImGuiIf::PlotLineG(const char* label_id,
    ImPlotGetter getter,
    void* data,
    int count,
    const ImPlotSpec& spec)
{
    (void)label_id;
    (void)getter;
    (void)data;
    (void)count;
    (void)spec;
}
void MockImGuiIf::PlotScatterG(const char* label_id,
    ImPlotGetter getter,
    void* data,
    int count,
    const ImPlotSpec& spec)
{
    (void)label_id;
    (void)getter;
    (void)data;
    (void)count;
    (void)spec;
}
void MockImGuiIf::PlotStairsG(const char* label_id,
    ImPlotGetter getter,
    void* data,
    int count,
    const ImPlotSpec& spec)
{
    (void)label_id;
    (void)getter;
    (void)data;
    (void)count;
    (void)spec;
}
void MockImGuiIf::PlotShadedG(const char* label_id,
    ImPlotGetter getter1,
    void* data1,
    ImPlotGetter getter2,
    void* data2,
    int count,
    const ImPlotSpec& spec)
{
    (void)label_id;
    (void)getter1;
    (void)data1;
    (void)getter2;
    (void)data2;
    (void)count;
    (void)spec;
}
void MockImGuiIf::PlotBarsG(const char* label_id,
    ImPlotGetter getter,
    void* data,
    int count,
    double bar_size,
    const ImPlotSpec& spec)
{
    (void)label_id;
    (void)getter;
    (void)data;
    (void)count;
    (void)bar_size;
    (void)spec;
}
void MockImGuiIf::PlotDigitalG(const char* label_id,
    ImPlotGetter getter,
    void* data,
    int count,
    const ImPlotSpec& spec)
{
    (void)label_id;
    (void)getter;
    (void)data;
    (void)count;
    (void)spec;
}
void MockImGuiIf::PlotImage(const char* label_id,
    ImTextureRef tex_ref,
    const ImPlotPoint& bounds_min,
    const ImPlotPoint& bounds_max,
    const ImVec2& uv0,
    const ImVec2& uv1,
    const ImVec4& tint_col,
    const ImPlotSpec& spec)
{
    (void)label_id;
    (void)tex_ref;
    (void)bounds_min;
    (void)bounds_max;
    (void)uv0;
    (void)uv1;
    (void)tint_col;
    (void)spec;
}
void MockImGuiIf::PlotImage(const char* label_id,
    ImTextureID tex_ref,
    const ImPlotPoint& bounds_min,
    const ImPlotPoint& bounds_max,
    const ImVec2& uv0,
    const ImVec2& uv1,
    const ImVec4& tint_col,
    const ImPlotSpec& spec)
{
    (void)label_id;
    (void)tex_ref;
    (void)bounds_min;
    (void)bounds_max;
    (void)uv0;
    (void)uv1;
    (void)tint_col;
    (void)spec;
}
void MockImGuiIf::PlotText(const char* text,
    double x,
    double y,
    const ImVec2& pix_offset,
    const ImPlotSpec& spec)
{
    (void)text;
    (void)x;
    (void)y;
    (void)pix_offset;
    (void)spec;
}
void MockImGuiIf::PlotDummy(const char* label_id, const ImPlotSpec& spec)
{
    (void)label_id;
    (void)spec;
}
bool MockImGuiIf::DragPoint(int id,
    double* x,
    double* y,
    const ImVec4& col,
    float size,
    ImPlotDragToolFlags flags,
    bool* out_clicked,
    bool* out_hovered,
    bool* out_held)
{
    (void)id;
    (void)x;
    (void)y;
    (void)col;
    (void)size;
    (void)flags;
    (void)out_clicked;
    (void)out_hovered;
    (void)out_held;
    return false;
}
bool MockImGuiIf::DragLineX(int id,
    double* x,
    const ImVec4& col,
    float thickness,
    ImPlotDragToolFlags flags,
    bool* out_clicked,
    bool* out_hovered,
    bool* out_held)
{
    (void)id;
    (void)x;
    (void)col;
    (void)thickness;
    (void)flags;
    (void)out_clicked;
    (void)out_hovered;
    (void)out_held;
    return false;
}
bool MockImGuiIf::DragLineY(int id,
    double* y,
    const ImVec4& col,
    float thickness,
    ImPlotDragToolFlags flags,
    bool* out_clicked,
    bool* out_hovered,
    bool* out_held)
{
    (void)id;
    (void)y;
    (void)col;
    (void)thickness;
    (void)flags;
    (void)out_clicked;
    (void)out_hovered;
    (void)out_held;
    return false;
}
bool MockImGuiIf::DragRect(int id,
    double* x1,
    double* y1,
    double* x2,
    double* y2,
    const ImVec4& col,
    ImPlotDragToolFlags flags,
    bool* out_clicked,
    bool* out_hovered,
    bool* out_held)
{
    (void)id;
    (void)x1;
    (void)y1;
    (void)x2;
    (void)y2;
    (void)col;
    (void)flags;
    (void)out_clicked;
    (void)out_hovered;
    (void)out_held;
    return false;
}
void MockImGuiIf::Annotation(double x,
    double y,
    const ImVec4& col,
    const ImVec2& pix_offset,
    bool clamp,
    bool round)
{
    (void)x;
    (void)y;
    (void)col;
    (void)pix_offset;
    (void)clamp;
    (void)round;
}
void MockImGuiIf::Annotation(double x,
    double y,
    const ImVec4& col,
    const ImVec2& pix_offset,
    bool clamp,
    const char* fmt,
    ...)
{
    (void)x;
    (void)y;
    (void)col;
    (void)pix_offset;
    (void)clamp;
    (void)fmt;
}
void MockImGuiIf::AnnotationV(double x,
    double y,
    const ImVec4& col,
    const ImVec2& pix_offset,
    bool clamp,
    const char* fmt,
    va_list args)
{
    (void)x;
    (void)y;
    (void)col;
    (void)pix_offset;
    (void)clamp;
    (void)fmt;
    (void)args;
}
void MockImGuiIf::TagX(double x, const ImVec4& col, bool round)
{
    (void)x;
    (void)col;
    (void)round;
}
void MockImGuiIf::TagX(double x, const ImVec4& col, const char* fmt, ...)
{
    (void)x;
    (void)col;
    (void)fmt;
}
void MockImGuiIf::TagXV(
    double x, const ImVec4& col, const char* fmt, va_list args)
{
    (void)x;
    (void)col;
    (void)fmt;
    (void)args;
}
void MockImGuiIf::TagY(double y, const ImVec4& col, bool round)
{
    (void)y;
    (void)col;
    (void)round;
}
void MockImGuiIf::TagY(double y, const ImVec4& col, const char* fmt, ...)
{
    (void)y;
    (void)col;
    (void)fmt;
}
void MockImGuiIf::TagYV(
    double y, const ImVec4& col, const char* fmt, va_list args)
{
    (void)y;
    (void)col;
    (void)fmt;
    (void)args;
}
void MockImGuiIf::SetAxis(ImAxis axis)
{
    (void)axis;
}
void MockImGuiIf::SetAxes(ImAxis x_axis, ImAxis y_axis)
{
    (void)x_axis;
    (void)y_axis;
}
ImPlotPoint MockImGuiIf::PixelsToPlot(
    const ImVec2& pix, ImAxis x_axis, ImAxis y_axis)
{
    (void)pix;
    (void)x_axis;
    (void)y_axis;
    return ImPlotPoint(0, 0);
}
ImPlotPoint MockImGuiIf::PixelsToPlot(
    float x, float y, ImAxis x_axis, ImAxis y_axis)
{
    (void)x;
    (void)y;
    (void)x_axis;
    (void)y_axis;
    return ImPlotPoint(0, 0);
}
ImVec2 MockImGuiIf::PlotToPixels(
    const ImPlotPoint& plt, ImAxis x_axis, ImAxis y_axis)
{
    (void)plt;
    (void)x_axis;
    (void)y_axis;
    return ImVec2(0, 0);
}
ImVec2 MockImGuiIf::PlotToPixels(
    double x, double y, ImAxis x_axis, ImAxis y_axis)
{
    (void)x;
    (void)y;
    (void)x_axis;
    (void)y_axis;
    return ImVec2(0, 0);
}
ImVec2 MockImGuiIf::GetPlotPos()
{
    return ImVec2(0, 0);
}
ImVec2 MockImGuiIf::GetPlotSize()
{
    return ImVec2(0, 0);
}
ImPlotPoint MockImGuiIf::GetPlotMousePos(ImAxis x_axis, ImAxis y_axis)
{
    (void)x_axis;
    (void)y_axis;
    return ImPlotPoint(0, 0);
}
ImPlotRect MockImGuiIf::GetPlotLimits(ImAxis x_axis, ImAxis y_axis)
{
    (void)x_axis;
    (void)y_axis;
    return ImPlotRect(0, 0, 0, 0);
}
bool MockImGuiIf::IsPlotHovered()
{
    return false;
}
bool MockImGuiIf::IsAxisHovered(ImAxis axis)
{
    (void)axis;
    return false;
}
bool MockImGuiIf::IsSubplotsHovered()
{
    return false;
}
bool MockImGuiIf::IsPlotSelected()
{
    return false;
}
ImPlotRect MockImGuiIf::GetPlotSelection(ImAxis x_axis, ImAxis y_axis)
{
    (void)x_axis;
    (void)y_axis;
    return ImPlotRect(0, 0, 0, 0);
}
void MockImGuiIf::CancelPlotSelection() {}
void MockImGuiIf::HideNextItem(bool hidden, ImPlotCond cond)
{
    (void)hidden;
    (void)cond;
}
bool MockImGuiIf::BeginAlignedPlots(const char* group_id, bool vertical)
{
    (void)group_id;
    (void)vertical;
    return false;
}
void MockImGuiIf::EndAlignedPlots() {}
bool MockImGuiIf::BeginLegendPopup(
    const char* label_id, ImGuiMouseButton mouse_button)
{
    (void)label_id;
    (void)mouse_button;
    return false;
}
void MockImGuiIf::EndLegendPopup() {}
bool MockImGuiIf::IsLegendEntryHovered(const char* label_id)
{
    (void)label_id;
    return false;
}
bool MockImGuiIf::BeginDragDropTargetPlot()
{
    return false;
}
bool MockImGuiIf::BeginDragDropTargetAxis(ImAxis axis)
{
    (void)axis;
    return false;
}
bool MockImGuiIf::BeginDragDropTargetLegend()
{
    return false;
}
void MockImGuiIf::ImPlot_EndDragDropTarget() {}
bool MockImGuiIf::BeginDragDropSourcePlot(ImGuiDragDropFlags flags)
{
    (void)flags;
    return false;
}
bool MockImGuiIf::BeginDragDropSourceAxis(ImAxis axis, ImGuiDragDropFlags flags)
{
    (void)axis;
    (void)flags;
    return false;
}
bool MockImGuiIf::BeginDragDropSourceItem(
    const char* label_id, ImGuiDragDropFlags flags)
{
    (void)label_id;
    (void)flags;
    return false;
}
void MockImGuiIf::ImPlot_EndDragDropSource() {}
ImPlotStyle& MockImGuiIf::ImPlot_GetStyle()
{
    return _plotStyle;
}
void MockImGuiIf::StyleColorsAuto(ImPlotStyle* dst)
{
    (void)dst;
}
void MockImGuiIf::ImPlot_StyleColorsClassic(ImPlotStyle* dst)
{
    (void)dst;
}
void MockImGuiIf::ImPlot_StyleColorsDark(ImPlotStyle* dst)
{
    (void)dst;
}
void MockImGuiIf::ImPlot_StyleColorsLight(ImPlotStyle* dst)
{
    (void)dst;
}
void MockImGuiIf::ImPlot_PushStyleColor(ImPlotCol idx, ImU32 col)
{
    (void)idx;
    (void)col;
}
void MockImGuiIf::ImPlot_PushStyleColor(ImPlotCol idx, const ImVec4& col)
{
    (void)idx;
    (void)col;
}
void MockImGuiIf::ImPlot_PopStyleColor(int count)
{
    (void)count;
}
void MockImGuiIf::ImPlot_PushStyleVar(ImPlotStyleVar idx, float val)
{
    (void)idx;
    (void)val;
}
void MockImGuiIf::ImPlot_PushStyleVar(ImPlotStyleVar idx, int val)
{
    (void)idx;
    (void)val;
}
void MockImGuiIf::ImPlot_PushStyleVar(ImPlotStyleVar idx, const ImVec2& val)
{
    (void)idx;
    (void)val;
}
void MockImGuiIf::ImPlot_PopStyleVar(int count)
{
    (void)count;
}
ImVec4 MockImGuiIf::GetLastItemColor()
{
    return ImVec4(0, 0, 0, 0);
}
const char* MockImGuiIf::ImPlot_GetStyleColorName(ImPlotCol idx)
{
    (void)idx;
    return nullptr;
}
const char* MockImGuiIf::GetMarkerName(ImPlotMarker idx)
{
    (void)idx;
    return nullptr;
}
ImPlotMarker MockImGuiIf::NextMarker()
{
    return 0;
}
ImPlotColormap MockImGuiIf::AddColormap(
    const char* name, const ImVec4* cols, int size, bool qual)
{
    (void)name;
    (void)cols;
    (void)size;
    (void)qual;
    return 0;
}
ImPlotColormap MockImGuiIf::AddColormap(
    const char* name, const ImU32* cols, int size, bool qual)
{
    (void)name;
    (void)cols;
    (void)size;
    (void)qual;
    return 0;
}
int MockImGuiIf::GetColormapCount()
{
    return 0;
}
const char* MockImGuiIf::GetColormapName(ImPlotColormap cmap)
{
    (void)cmap;
    return nullptr;
}
ImPlotColormap MockImGuiIf::GetColormapIndex(const char* name)
{
    (void)name;
    return 0;
}
void MockImGuiIf::PushColormap(ImPlotColormap cmap)
{
    (void)cmap;
}
void MockImGuiIf::PushColormap(const char* name)
{
    (void)name;
}
void MockImGuiIf::PopColormap(int count)
{
    (void)count;
}
ImVec4 MockImGuiIf::NextColormapColor()
{
    return ImVec4(0, 0, 0, 0);
}
int MockImGuiIf::GetColormapSize(ImPlotColormap cmap)
{
    (void)cmap;
    return 0;
}
ImVec4 MockImGuiIf::GetColormapColor(int idx, ImPlotColormap cmap)
{
    (void)idx;
    (void)cmap;
    return ImVec4(0, 0, 0, 0);
}
ImVec4 MockImGuiIf::SampleColormap(float t, ImPlotColormap cmap)
{
    (void)t;
    (void)cmap;
    return ImVec4(0, 0, 0, 0);
}
void MockImGuiIf::ColormapScale(const char* label,
    double scale_min,
    double scale_max,
    const ImVec2& size,
    const char* format,
    ImPlotColormapScaleFlags flags,
    ImPlotColormap cmap)
{
    (void)label;
    (void)scale_min;
    (void)scale_max;
    (void)size;
    (void)format;
    (void)flags;
    (void)cmap;
}
bool MockImGuiIf::ColormapSlider(const char* label,
    float* t,
    ImVec4* out,
    const char* format,
    ImPlotColormap cmap)
{
    (void)label;
    (void)t;
    (void)out;
    (void)format;
    (void)cmap;
    return false;
}
bool MockImGuiIf::ColormapButton(
    const char* label, const ImVec2& size, ImPlotColormap cmap)
{
    (void)label;
    (void)size;
    (void)cmap;
    return false;
}
void MockImGuiIf::BustColorCache(const char* plot_title_id)
{
    (void)plot_title_id;
}
ImPlotInputMap& MockImGuiIf::GetInputMap()
{
    return _inputMap;
}
void MockImGuiIf::MapInputDefault(ImPlotInputMap* dst)
{
    (void)dst;
}
void MockImGuiIf::MapInputReverse(ImPlotInputMap* dst)
{
    (void)dst;
}
void MockImGuiIf::ItemIcon(const ImVec4& col)
{
    (void)col;
}
void MockImGuiIf::ItemIcon(ImU32 col)
{
    (void)col;
}
void MockImGuiIf::ColormapIcon(ImPlotColormap cmap)
{
    (void)cmap;
}
ImDrawList* MockImGuiIf::GetPlotDrawList()
{
    return _dummyDrawList;
}
void MockImGuiIf::PushPlotClipRect(float expand)
{
    (void)expand;
}
void MockImGuiIf::PopPlotClipRect() {}
bool MockImGuiIf::ImPlot_ShowStyleSelector(const char* label)
{
    (void)label;
    return false;
}
bool MockImGuiIf::ShowColormapSelector(const char* label)
{
    (void)label;
    return false;
}
bool MockImGuiIf::ShowInputMapSelector(const char* label)
{
    (void)label;
    return false;
}
void MockImGuiIf::ImPlot_ShowStyleEditor(ImPlotStyle* ref)
{
    (void)ref;
}
void MockImGuiIf::ImPlot_ShowUserGuide() {}
void MockImGuiIf::ImPlot_ShowMetricsWindow(bool* p_popen)
{
    (void)p_popen;
}
void MockImGuiIf::ImPlot_ShowDemoWindow(bool* p_open)
{
    (void)p_open;
}
void MockImGuiIf::SetColorEditOptions(ImGuiColorEditFlags flags)
{
    (void)flags;
}
void MockImGuiIf::SetWindowFontScale(float scale)
{
    (void)scale;
}
void MockImGuiIf::Image(ImTextureRef tex_ref,
    const ImVec2& image_size,
    const ImVec2& uv0,
    const ImVec2& uv1,
    const ImVec4& tint_col,
    const ImVec4& border_col)
{
    (void)tex_ref;
    (void)image_size;
    (void)uv0;
    (void)uv1;
    (void)tint_col;
    (void)border_col;
}
ImVec2 MockImGuiIf::GetContentRegionMax()
{
    return ImVec2(0, 0);
}
ImVec2 MockImGuiIf::GetWindowContentRegionMin()
{
    return ImVec2(0, 0);
}
ImVec2 MockImGuiIf::GetWindowContentRegionMax()
{
    return ImVec2(0, 0);
}