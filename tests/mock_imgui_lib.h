#pragma once

#include <cstdarg>
#include <imgui.h>
#include <imgui_internal.h>
#include <implot.h>
#include "imguiif.h"

class MockImGuiIf : public ImGuiIf
{
public:
    MockImGuiIf();
    ~MockImGuiIf() override;

    // ImGuiIf overrides
    ImGuiContext* CreateContext(ImFontAtlas* shared_font_atlas = NULL) override;
    void DestroyContext(ImGuiContext* ctx = NULL) override;
    ImGuiContext* GetCurrentContext() override;
    void SetCurrentContext(ImGuiContext* ctx) override;
    ImGuiIO& GetIO() override;
    ImGuiPlatformIO& GetPlatformIO() override;
    ImGuiStyle& GetStyle() override;
    void NewFrame() override;
    void EndFrame() override;
    void Render() override;
    ImDrawData* GetDrawData() override;
    void ShowDemoWindow(bool* p_open = NULL) override;
    void ShowMetricsWindow(bool* p_open = NULL) override;
    void ShowDebugLogWindow(bool* p_open = NULL) override;
    void ShowIDStackToolWindow(bool* p_open = NULL) override;
    void ShowAboutWindow(bool* p_open = NULL) override;
    void ShowStyleEditor(ImGuiStyle* ref = NULL) override;
    bool ShowStyleSelector(const char* label) override;
    void ShowFontSelector(const char* label) override;
    void ShowUserGuide() override;
    const char* GetVersion() override;
    void StyleColorsDark(ImGuiStyle* dst = NULL) override;
    void StyleColorsLight(ImGuiStyle* dst = NULL) override;
    void StyleColorsClassic(ImGuiStyle* dst = NULL) override;
    bool Begin(const char* name,
        bool* p_open = NULL,
        ImGuiWindowFlags flags = 0) override;
    void End() override;
    bool BeginChild(const char* str_id,
        const ImVec2& size = ImVec2(0, 0),
        ImGuiChildFlags child_flags = 0,
        ImGuiWindowFlags window_flags = 0) override;
    bool BeginChild(ImGuiID id,
        const ImVec2& size = ImVec2(0, 0),
        ImGuiChildFlags child_flags = 0,
        ImGuiWindowFlags window_flags = 0) override;
    void EndChild() override;
    bool IsWindowAppearing() override;
    bool IsWindowCollapsed() override;
    bool IsWindowFocused(ImGuiFocusedFlags flags = 0) override;
    bool IsWindowHovered(ImGuiHoveredFlags flags = 0) override;
    ImDrawList* GetWindowDrawList() override;
    float GetWindowDpiScale() override;
    ImVec2 GetWindowPos() override;
    ImVec2 GetWindowSize() override;
    float GetWindowWidth() override;
    float GetWindowHeight() override;
    ImGuiViewport* GetWindowViewport() override;
    void SetNextWindowPos(const ImVec2& pos,
        ImGuiCond cond = 0,
        const ImVec2& pivot = ImVec2(0, 0)) override;
    void SetNextWindowSize(const ImVec2& size, ImGuiCond cond = 0) override;
    void SetNextWindowSizeConstraints(const ImVec2& size_min,
        const ImVec2& size_max,
        ImGuiSizeCallback custom_callback = NULL,
        void* custom_callback_data = NULL) override;
    void SetNextWindowContentSize(const ImVec2& size) override;
    void SetNextWindowCollapsed(bool collapsed, ImGuiCond cond = 0) override;
    void SetNextWindowFocus() override;
    void SetNextWindowScroll(const ImVec2& scroll) override;
    void SetNextWindowBgAlpha(float alpha) override;
    void SetNextWindowViewport(ImGuiID viewport_id) override;
    void SetWindowPos(const ImVec2& pos, ImGuiCond cond = 0) override;
    void SetWindowSize(const ImVec2& size, ImGuiCond cond = 0) override;
    void SetWindowCollapsed(bool collapsed, ImGuiCond cond = 0) override;
    void SetWindowFocus() override;
    void SetWindowPos(
        const char* name, const ImVec2& pos, ImGuiCond cond = 0) override;
    void SetWindowSize(
        const char* name, const ImVec2& size, ImGuiCond cond = 0) override;
    void SetWindowCollapsed(
        const char* name, bool collapsed, ImGuiCond cond = 0) override;
    void SetWindowFocus(const char* name) override;
    float GetScrollX() override;
    float GetScrollY() override;
    void SetScrollX(float scroll_x) override;
    void SetScrollY(float scroll_y) override;
    float GetScrollMaxX() override;
    float GetScrollMaxY() override;
    void SetScrollHereX(float center_x_ratio = 0.5f) override;
    void SetScrollHereY(float center_y_ratio = 0.5f) override;
    void SetScrollFromPosX(float local_x, float center_x_ratio = 0.5f) override;
    void SetScrollFromPosY(float local_y, float center_y_ratio = 0.5f) override;
    void PushFont(ImFont* font, float font_size_base_unscaled) override;
    void PopFont() override;
    ImFont* GetFont() override;
    float GetFontSize() override;
    ImFontBaked* GetFontBaked() override;
    void PushStyleColor(ImGuiCol idx, ImU32 col) override;
    void PushStyleColor(ImGuiCol idx, const ImVec4& col) override;
    void PopStyleColor(int count = 1) override;
    void PushStyleVar(ImGuiStyleVar idx, float val) override;
    void PushStyleVar(ImGuiStyleVar idx, const ImVec2& val) override;
    void PushStyleVarX(ImGuiStyleVar idx, float val_x) override;
    void PushStyleVarY(ImGuiStyleVar idx, float val_y) override;
    void PopStyleVar(int count = 1) override;
    void PushItemFlag(ImGuiItemFlags option, bool enabled) override;
    void PopItemFlag() override;
    void PushItemWidth(float item_width) override;
    void PopItemWidth() override;
    void SetNextItemWidth(float item_width) override;
    float CalcItemWidth() override;
    void PushTextWrapPos(float wrap_local_pos_x = 0.0f) override;
    void PopTextWrapPos() override;
    ImVec2 GetFontTexUvWhitePixel() override;
    ImU32 GetColorU32(ImGuiCol idx, float alpha_mul = 1.0f) override;
    ImU32 GetColorU32(const ImVec4& col) override;
    ImU32 GetColorU32(ImU32 col, float alpha_mul = 1.0f) override;
    const ImVec4& GetStyleColorVec4(ImGuiCol idx) override;
    ImVec2 GetCursorScreenPos() override;
    void SetCursorScreenPos(const ImVec2& pos) override;
    ImVec2 GetContentRegionAvail() override;
    ImVec2 GetCursorPos() override;
    float GetCursorPosX() override;
    float GetCursorPosY() override;
    void SetCursorPos(const ImVec2& local_pos) override;
    void SetCursorPosX(float local_x) override;
    void SetCursorPosY(float local_y) override;
    ImVec2 GetCursorStartPos() override;
    void Separator() override;
    void SameLine(
        float offset_from_start_x = 0.0f, float spacing = -1.0f) override;
    void NewLine() override;
    void Spacing() override;
    void Dummy(const ImVec2& size) override;
    void Indent(float indent_w = 0.0f) override;
    void Unindent(float indent_w = 0.0f) override;
    void BeginGroup() override;
    void EndGroup() override;
    void AlignTextToFramePadding() override;
    float GetTextLineHeight() override;
    float GetTextLineHeightWithSpacing() override;
    float GetFrameHeight() override;
    float GetFrameHeightWithSpacing() override;
    void PushID(const char* str_id) override;
    void PushID(const char* str_id_begin, const char* str_id_end) override;
    void PushID(const void* ptr_id) override;
    void PushID(int int_id) override;
    void PopID() override;
    ImGuiID GetID(const char* str_id) override;
    ImGuiID GetID(const char* str_id_begin, const char* str_id_end) override;
    ImGuiID GetID(const void* ptr_id) override;
    ImGuiID GetID(int int_id) override;
    void TextUnformatted(
        const char* text, const char* text_end = NULL) override;
    void Text(const char* fmt, ...) override;
    void TextV(const char* fmt, va_list args) override;
    void TextColored(const ImVec4& col, const char* fmt, ...) override;
    void TextColoredV(
        const ImVec4& col, const char* fmt, va_list args) override;
    void TextDisabled(const char* fmt, ...) override;
    void TextDisabledV(const char* fmt, va_list args) override;
    void TextWrapped(const char* fmt, ...) override;
    void TextWrappedV(const char* fmt, va_list args) override;
    void LabelText(const char* label, const char* fmt, ...) override;
    void LabelTextV(const char* label, const char* fmt, va_list args) override;
    void BulletText(const char* fmt, ...) override;
    void BulletTextV(const char* fmt, va_list args) override;
    void SeparatorText(const char* label) override;
    bool Button(const char* label, const ImVec2& size = ImVec2(0, 0)) override;
    bool SmallButton(const char* label) override;
    bool InvisibleButton(const char* str_id,
        const ImVec2& size,
        ImGuiButtonFlags flags = 0) override;
    bool ArrowButton(const char* str_id, ImGuiDir dir) override;
    bool Checkbox(const char* label, bool* v) override;
    bool CheckboxFlags(const char* label, int* flags, int flags_value) override;
    bool CheckboxFlags(const char* label,
        unsigned int* flags,
        unsigned int flags_value) override;
    bool RadioButton(const char* label, bool active) override;
    bool RadioButton(const char* label, int* v, int v_button) override;
    void ProgressBar(float fraction,
        const ImVec2& size_arg = ImVec2(-FLT_MIN, 0),
        const char* overlay = NULL) override;
    void Bullet() override;
    bool TextLink(const char* label) override;
    bool TextLinkOpenURL(const char* label, const char* url = NULL) override;
    void Image(ImTextureRef tex_ref,
        const ImVec2& image_size,
        const ImVec2& uv0 = ImVec2(0, 0),
        const ImVec2& uv1 = ImVec2(1, 1)) override;
    void ImageWithBg(ImTextureRef tex_ref,
        const ImVec2& image_size,
        const ImVec2& uv0 = ImVec2(0, 0),
        const ImVec2& uv1 = ImVec2(1, 1),
        const ImVec4& bg_col = ImVec4(0, 0, 0, 0),
        const ImVec4& tint_col = ImVec4(1, 1, 1, 1)) override;
    bool ImageButton(const char* str_id,
        ImTextureRef tex_ref,
        const ImVec2& image_size,
        const ImVec2& uv0 = ImVec2(0, 0),
        const ImVec2& uv1 = ImVec2(1, 1),
        const ImVec4& bg_col = ImVec4(0, 0, 0, 0),
        const ImVec4& tint_col = ImVec4(1, 1, 1, 1)) override;
    bool BeginCombo(const char* label,
        const char* preview_value,
        ImGuiComboFlags flags = 0) override;
    void EndCombo() override;
    bool Combo(const char* label,
        int* current_item,
        const char* const items[],
        int items_count,
        int popup_max_height_in_items = -1) override;
    bool Combo(const char* label,
        int* current_item,
        const char* items_separated_by_zeros,
        int popup_max_height_in_items = -1) override;
    bool Combo(const char* label,
        int* current_item,
        const char* (*getter)(void* user_data, int idx),
        void* user_data,
        int items_count,
        int popup_max_height_in_items = -1) override;
    bool DragFloat(const char* label,
        float* v,
        float v_speed = 1.0f,
        float v_min = 0.0f,
        float v_max = 0.0f,
        const char* format = "%.3f",
        ImGuiSliderFlags flags = 0) override;
    bool DragFloat2(const char* label,
        float v[2],
        float v_speed = 1.0f,
        float v_min = 0.0f,
        float v_max = 0.0f,
        const char* format = "%.3f",
        ImGuiSliderFlags flags = 0) override;
    bool DragFloat3(const char* label,
        float v[3],
        float v_speed = 1.0f,
        float v_min = 0.0f,
        float v_max = 0.0f,
        const char* format = "%.3f",
        ImGuiSliderFlags flags = 0) override;
    bool DragFloat4(const char* label,
        float v[4],
        float v_speed = 1.0f,
        float v_min = 0.0f,
        float v_max = 0.0f,
        const char* format = "%.3f",
        ImGuiSliderFlags flags = 0) override;
    bool DragFloatRange2(const char* label,
        float* v_current_min,
        float* v_current_max,
        float v_speed = 1.0f,
        float v_min = 0.0f,
        float v_max = 0.0f,
        const char* format = "%.3f",
        const char* format_max = NULL,
        ImGuiSliderFlags flags = 0) override;
    bool DragInt(const char* label,
        int* v,
        float v_speed = 1.0f,
        int v_min = 0,
        int v_max = 0,
        const char* format = "%d",
        ImGuiSliderFlags flags = 0) override;
    bool DragInt2(const char* label,
        int v[2],
        float v_speed = 1.0f,
        int v_min = 0,
        int v_max = 0,
        const char* format = "%d",
        ImGuiSliderFlags flags = 0) override;
    bool DragInt3(const char* label,
        int v[3],
        float v_speed = 1.0f,
        int v_min = 0,
        int v_max = 0,
        const char* format = "%d",
        ImGuiSliderFlags flags = 0) override;
    bool DragInt4(const char* label,
        int v[4],
        float v_speed = 1.0f,
        int v_min = 0,
        int v_max = 0,
        const char* format = "%d",
        ImGuiSliderFlags flags = 0) override;
    bool DragIntRange2(const char* label,
        int* v_current_min,
        int* v_current_max,
        float v_speed = 1.0f,
        int v_min = 0,
        int v_max = 0,
        const char* format = "%d",
        const char* format_max = NULL,
        ImGuiSliderFlags flags = 0) override;
    bool DragScalar(const char* label,
        ImGuiDataType data_type,
        void* p_data,
        float v_speed = 1.0f,
        const void* p_min = NULL,
        const void* p_max = NULL,
        const char* format = NULL,
        ImGuiSliderFlags flags = 0) override;
    bool DragScalarN(const char* label,
        ImGuiDataType data_type,
        void* p_data,
        int components,
        float v_speed = 1.0f,
        const void* p_min = NULL,
        const void* p_max = NULL,
        const char* format = NULL,
        ImGuiSliderFlags flags = 0) override;
    bool SliderFloat(const char* label,
        float* v,
        float v_min,
        float v_max,
        const char* format = "%.3f",
        ImGuiSliderFlags flags = 0) override;
    bool SliderFloat2(const char* label,
        float v[2],
        float v_min,
        float v_max,
        const char* format = "%.3f",
        ImGuiSliderFlags flags = 0) override;
    bool SliderFloat3(const char* label,
        float v[3],
        float v_min,
        float v_max,
        const char* format = "%.3f",
        ImGuiSliderFlags flags = 0) override;
    bool SliderFloat4(const char* label,
        float v[4],
        float v_min,
        float v_max,
        const char* format = "%.3f",
        ImGuiSliderFlags flags = 0) override;
    bool SliderAngle(const char* label,
        float* v_rad,
        float v_degrees_min = -360.0f,
        float v_degrees_max = +360.0f,
        const char* format = "%.0f deg",
        ImGuiSliderFlags flags = 0) override;
    bool SliderInt(const char* label,
        int* v,
        int v_min,
        int v_max,
        const char* format = "%d",
        ImGuiSliderFlags flags = 0) override;
    bool SliderInt2(const char* label,
        int v[2],
        int v_min,
        int v_max,
        const char* format = "%d",
        ImGuiSliderFlags flags = 0) override;
    bool SliderInt3(const char* label,
        int v[3],
        int v_min,
        int v_max,
        const char* format = "%d",
        ImGuiSliderFlags flags = 0) override;
    bool SliderInt4(const char* label,
        int v[4],
        int v_min,
        int v_max,
        const char* format = "%d",
        ImGuiSliderFlags flags = 0) override;
    bool SliderScalar(const char* label,
        ImGuiDataType data_type,
        void* p_data,
        const void* p_min,
        const void* p_max,
        const char* format = NULL,
        ImGuiSliderFlags flags = 0) override;
    bool SliderScalarN(const char* label,
        ImGuiDataType data_type,
        void* p_data,
        int components,
        const void* p_min,
        const void* p_max,
        const char* format = NULL,
        ImGuiSliderFlags flags = 0) override;
    bool VSliderFloat(const char* label,
        const ImVec2& size,
        float* v,
        float v_min,
        float v_max,
        const char* format = "%.3f",
        ImGuiSliderFlags flags = 0) override;
    bool VSliderInt(const char* label,
        const ImVec2& size,
        int* v,
        int v_min,
        int v_max,
        const char* format = "%d",
        ImGuiSliderFlags flags = 0) override;
    bool VSliderScalar(const char* label,
        const ImVec2& size,
        ImGuiDataType data_type,
        void* p_data,
        const void* p_min,
        const void* p_max,
        const char* format = NULL,
        ImGuiSliderFlags flags = 0) override;
    bool InputText(const char* label,
        char* buf,
        size_t buf_size,
        ImGuiInputTextFlags flags = 0,
        ImGuiInputTextCallback callback = NULL,
        void* user_data = NULL) override;
    bool InputTextMultiline(const char* label,
        char* buf,
        size_t buf_size,
        const ImVec2& size = ImVec2(0, 0),
        ImGuiInputTextFlags flags = 0,
        ImGuiInputTextCallback callback = NULL,
        void* user_data = NULL) override;
    bool InputTextWithHint(const char* label,
        const char* hint,
        char* buf,
        size_t buf_size,
        ImGuiInputTextFlags flags = 0,
        ImGuiInputTextCallback callback = NULL,
        void* user_data = NULL) override;
    bool InputFloat(const char* label,
        float* v,
        float step = 0.0f,
        float step_fast = 0.0f,
        const char* format = "%.3f",
        ImGuiInputTextFlags flags = 0) override;
    bool InputFloat2(const char* label,
        float v[2],
        const char* format = "%.3f",
        ImGuiInputTextFlags flags = 0) override;
    bool InputFloat3(const char* label,
        float v[3],
        const char* format = "%.3f",
        ImGuiInputTextFlags flags = 0) override;
    bool InputFloat4(const char* label,
        float v[4],
        const char* format = "%.3f",
        ImGuiInputTextFlags flags = 0) override;
    bool InputInt(const char* label,
        int* v,
        int step = 1,
        int step_fast = 100,
        ImGuiInputTextFlags flags = 0) override;
    bool InputInt2(
        const char* label, int v[2], ImGuiInputTextFlags flags = 0) override;
    bool InputInt3(
        const char* label, int v[3], ImGuiInputTextFlags flags = 0) override;
    bool InputInt4(
        const char* label, int v[4], ImGuiInputTextFlags flags = 0) override;
    bool InputDouble(const char* label,
        double* v,
        double step = 0.0,
        double step_fast = 0.0,
        const char* format = "%.6f",
        ImGuiInputTextFlags flags = 0) override;
    bool InputScalar(const char* label,
        ImGuiDataType data_type,
        void* p_data,
        const void* p_step = NULL,
        const void* p_step_fast = NULL,
        const char* format = NULL,
        ImGuiInputTextFlags flags = 0) override;
    bool InputScalarN(const char* label,
        ImGuiDataType data_type,
        void* p_data,
        int components,
        const void* p_step = NULL,
        const void* p_step_fast = NULL,
        const char* format = NULL,
        ImGuiInputTextFlags flags = 0) override;
    bool ColorEdit3(const char* label,
        float col[3],
        ImGuiColorEditFlags flags = 0) override;
    bool ColorEdit4(const char* label,
        float col[4],
        ImGuiColorEditFlags flags = 0) override;
    bool ColorPicker3(const char* label,
        float col[3],
        ImGuiColorEditFlags flags = 0) override;
    bool ColorPicker4(const char* label,
        float col[4],
        ImGuiColorEditFlags flags = 0,
        const float* ref_col = NULL) override;
    bool ColorButton(const char* desc_id,
        const ImVec4& col,
        ImGuiColorEditFlags flags = 0,
        const ImVec2& size = ImVec2(0, 0)) override;
    bool TreeNode(const char* label) override;
    bool TreeNode(const char* str_id, const char* fmt, ...) override;
    bool TreeNode(const void* ptr_id, const char* fmt, ...) override;
    bool TreeNodeV(const char* str_id, const char* fmt, va_list args) override;
    bool TreeNodeV(const void* ptr_id, const char* fmt, va_list args) override;
    bool TreeNodeEx(const char* label, ImGuiTreeNodeFlags flags = 0) override;
    bool TreeNodeEx(const char* str_id,
        ImGuiTreeNodeFlags flags,
        const char* fmt,
        ...) override;
    bool TreeNodeEx(const void* ptr_id,
        ImGuiTreeNodeFlags flags,
        const char* fmt,
        ...) override;
    bool TreeNodeExV(const char* str_id,
        ImGuiTreeNodeFlags flags,
        const char* fmt,
        va_list args) override;
    bool TreeNodeExV(const void* ptr_id,
        ImGuiTreeNodeFlags flags,
        const char* fmt,
        va_list args) override;
    void TreePush(const char* str_id) override;
    void TreePush(const void* ptr_id) override;
    void TreePop() override;
    float GetTreeNodeToLabelSpacing() override;
    bool CollapsingHeader(
        const char* label, ImGuiTreeNodeFlags flags = 0) override;
    bool CollapsingHeader(const char* label,
        bool* p_visible,
        ImGuiTreeNodeFlags flags = 0) override;
    void SetNextItemOpen(bool is_open, ImGuiCond cond = 0) override;
    void SetNextItemStorageID(ImGuiID storage_id) override;
    bool TreeNodeGetOpen(ImGuiID storage_id) override;
    bool Selectable(const char* label,
        bool selected = false,
        ImGuiSelectableFlags flags = 0,
        const ImVec2& size = ImVec2(0, 0)) override;
    bool Selectable(const char* label,
        bool* p_selected,
        ImGuiSelectableFlags flags = 0,
        const ImVec2& size = ImVec2(0, 0)) override;
    ImGuiMultiSelectIO* BeginMultiSelect(ImGuiMultiSelectFlags flags,
        int selection_size = -1,
        int items_count = -1) override;
    ImGuiMultiSelectIO* EndMultiSelect() override;
    void SetNextItemSelectionUserData(
        ImGuiSelectionUserData selection_user_data) override;
    bool IsItemToggledSelection() override;
    bool BeginListBox(
        const char* label, const ImVec2& size = ImVec2(0, 0)) override;
    void EndListBox() override;
    bool ListBox(const char* label,
        int* current_item,
        const char* const items[],
        int items_count,
        int height_in_items = -1) override;
    bool ListBox(const char* label,
        int* current_item,
        const char* (*getter)(void* user_data, int idx),
        void* user_data,
        int items_count,
        int height_in_items = -1) override;
    void PlotLines(const char* label,
        const float* values,
        int values_count,
        int values_offset = 0,
        const char* overlay_text = NULL,
        float scale_min = FLT_MAX,
        float scale_max = FLT_MAX,
        ImVec2 graph_size = ImVec2(0, 0),
        int stride = sizeof(float)) override;
    void PlotLines(const char* label,
        float (*values_getter)(void* data, int idx),
        void* data,
        int values_count,
        int values_offset = 0,
        const char* overlay_text = NULL,
        float scale_min = FLT_MAX,
        float scale_max = FLT_MAX,
        ImVec2 graph_size = ImVec2(0, 0)) override;
    void PlotHistogram(const char* label,
        const float* values,
        int values_count,
        int values_offset = 0,
        const char* overlay_text = NULL,
        float scale_min = FLT_MAX,
        float scale_max = FLT_MAX,
        ImVec2 graph_size = ImVec2(0, 0),
        int stride = sizeof(float)) override;
    void PlotHistogram(const char* label,
        float (*values_getter)(void* data, int idx),
        void* data,
        int values_count,
        int values_offset = 0,
        const char* overlay_text = NULL,
        float scale_min = FLT_MAX,
        float scale_max = FLT_MAX,
        ImVec2 graph_size = ImVec2(0, 0)) override;
    void Value(const char* prefix, bool b) override;
    void Value(const char* prefix, int v) override;
    void Value(const char* prefix, unsigned int v) override;
    void Value(
        const char* prefix, float v, const char* float_format = NULL) override;
    bool BeginMenuBar() override;
    void EndMenuBar() override;
    bool BeginMainMenuBar() override;
    void EndMainMenuBar() override;
    bool BeginMenu(const char* label, bool enabled = true) override;
    void EndMenu() override;
    bool MenuItem(const char* label,
        const char* shortcut = NULL,
        bool selected = false,
        bool enabled = true) override;
    bool MenuItem(const char* label,
        const char* shortcut,
        bool* p_selected,
        bool enabled = true) override;
    bool BeginTooltip() override;
    void EndTooltip() override;
    void SetTooltip(const char* fmt, ...) override;
    void SetTooltipV(const char* fmt, va_list args) override;
    bool BeginItemTooltip() override;
    void SetItemTooltip(const char* fmt, ...) override;
    void SetItemTooltipV(const char* fmt, va_list args) override;
    bool BeginPopup(const char* str_id, ImGuiWindowFlags flags = 0) override;
    bool BeginPopupModal(const char* name,
        bool* p_open = NULL,
        ImGuiWindowFlags flags = 0) override;
    void EndPopup() override;
    bool OpenPopup(
        const char* str_id, ImGuiPopupFlags popup_flags = 0) override;
    bool OpenPopup(ImGuiID id, ImGuiPopupFlags popup_flags = 0) override;
    bool OpenPopupOnItemClick(
        const char* str_id = NULL, ImGuiPopupFlags popup_flags = 0) override;
    void CloseCurrentPopup() override;
    bool BeginPopupContextItem(
        const char* str_id = NULL, ImGuiPopupFlags popup_flags = 0) override;
    bool BeginPopupContextWindow(
        const char* str_id = NULL, ImGuiPopupFlags popup_flags = 0) override;
    bool BeginPopupContextVoid(
        const char* str_id = NULL, ImGuiPopupFlags popup_flags = 0) override;
    bool IsPopupOpen(const char* str_id, ImGuiPopupFlags flags = 0) override;
    bool BeginTable(const char* str_id,
        int columns,
        ImGuiTableFlags flags = 0,
        const ImVec2& outer_size = ImVec2(0.0f, 0.0f),
        float inner_width = 0.0f) override;
    void EndTable() override;
    void TableNextRow(
        ImGuiTableRowFlags row_flags = 0, float min_row_height = 0.0f) override;
    bool TableNextColumn() override;
    bool TableSetColumnIndex(int column_n) override;
    void TableSetupColumn(const char* label,
        ImGuiTableColumnFlags flags = 0,
        float init_width_or_weight = 0.0f,
        ImGuiID user_data = 0) override;
    void TableSetupScrollFreeze(int cols, int rows) override;
    void TableHeader(const char* label) override;
    void TableHeadersRow() override;
    void TableAngledHeadersRow() override;
    ImGuiTableSortSpecs* TableGetSortSpecs() override;
    int TableGetColumnCount() override;
    int TableGetColumnIndex() override;
    int TableGetRowIndex() override;
    const char* TableGetColumnName(int column_n = -1) override;
    ImGuiTableColumnFlags TableGetColumnFlags(int column_n = -1) override;
    void TableSetColumnEnabled(int column_n, bool v) override;
    int TableGetHoveredColumn() override;
    void TableSetBgColor(
        ImGuiTableBgTarget target, ImU32 color, int column_n = -1) override;
    void Columns(
        int count = 1, const char* id = NULL, bool borders = true) override;
    void NextColumn() override;
    int GetColumnIndex() override;
    float GetColumnWidth(int column_index = -1) override;
    void SetColumnWidth(int column_index, float width) override;
    float GetColumnOffset(int column_index = -1) override;
    void SetColumnOffset(int column_index, float offset_x) override;
    int GetColumnsCount() override;
    bool BeginTabBar(const char* str_id, ImGuiTabBarFlags flags = 0) override;
    void EndTabBar() override;
    bool BeginTabItem(const char* label,
        bool* p_open = NULL,
        ImGuiTabItemFlags flags = 0) override;
    void EndTabItem() override;
    bool TabItemButton(const char* label, ImGuiTabItemFlags flags = 0) override;
    void SetTabItemClosed(const char* tab_or_docked_window_label) override;
    ImGuiID DockSpace(ImGuiID dockspace_id,
        const ImVec2& size = ImVec2(0, 0),
        ImGuiDockNodeFlags flags = 0,
        const ImGuiWindowClass* window_class = NULL) override;
    ImGuiID DockSpaceOverViewport(ImGuiID dockspace_id = 0,
        const ImGuiViewport* viewport = NULL,
        ImGuiDockNodeFlags flags = 0,
        const ImGuiWindowClass* window_class = NULL) override;
    void SetNextWindowDockID(ImGuiID dock_id, ImGuiCond cond = 0) override;
    void SetNextWindowClass(const ImGuiWindowClass* window_class) override;
    ImGuiID GetWindowDockID() override;
    bool IsWindowDocked() override;
    void LogToTTY(int auto_open_depth = -1) override;
    void LogToFile(
        int auto_open_depth = -1, const char* filename = NULL) override;
    void LogToClipboard(int auto_open_depth = -1) override;
    void LogFinish() override;
    void LogButtons() override;
    void LogText(const char* fmt, ...) override;
    void LogTextV(const char* fmt, va_list args) override;
    bool BeginDragDropSource(ImGuiDragDropFlags flags = 0) override;
    bool SetDragDropPayload(const char* type,
        const void* data,
        size_t sz,
        ImGuiCond cond = 0) override;
    void EndDragDropSource() override;
    bool BeginDragDropTarget() override;
    const ImGuiPayload* AcceptDragDropPayload(
        const char* type, ImGuiDragDropFlags flags = 0) override;
    void EndDragDropTarget() override;
    const ImGuiPayload* GetDragDropPayload() override;
    void BeginDisabled(bool disabled = true) override;
    void EndDisabled() override;
    void PushClipRect(const ImVec2& clip_rect_min,
        const ImVec2& clip_rect_max,
        bool intersect_with_current_clip_rect) override;
    void PopClipRect() override;
    void SetItemDefaultFocus() override;
    void SetKeyboardFocusHere(int offset = 0) override;
    void SetNavCursorVisible(bool visible) override;
    void SetNextItemAllowOverlap() override;
    bool IsItemHovered(ImGuiHoveredFlags flags = 0) override;
    bool IsItemActive() override;
    bool IsItemFocused() override;
    bool IsItemClicked(ImGuiMouseButton mouse_button = 0) override;
    bool IsItemVisible() override;
    bool IsItemEdited() override;
    bool IsItemActivated() override;
    bool IsItemDeactivated() override;
    bool IsItemDeactivatedAfterEdit() override;
    bool IsItemToggledOpen() override;
    bool IsAnyItemHovered() override;
    bool IsAnyItemActive() override;
    bool IsAnyItemFocused() override;
    ImGuiID GetItemID() override;
    ImVec2 GetItemRectMin() override;
    ImVec2 GetItemRectMax() override;
    ImVec2 GetItemRectSize() override;
    ImGuiItemFlags GetItemFlags() override;
    int GetItemClickedCountWithSingleClickDelay(
        ImGuiMouseButton mouse_button = 0, float delay = -1.0f) override;
    ImGuiViewport* GetMainViewport() override;
    ImDrawList* GetBackgroundDrawList(ImGuiViewport* viewport = NULL) override;
    ImDrawList* GetForegroundDrawList(ImGuiViewport* viewport = NULL) override;
    bool IsRectVisible(const ImVec2& size) override;
    bool IsRectVisible(const ImVec2& rect_min, const ImVec2& rect_max) override;
    double GetTime() override;
    int GetFrameCount() override;
    ImDrawListSharedData* GetDrawListSharedData() override;
    const char* GetStyleColorName(ImGuiCol idx) override;
    void SetStateStorage(ImGuiStorage* storage) override;
    ImGuiStorage* GetStateStorage() override;
    ImVec2 CalcTextSize(const char* text,
        const char* text_end = NULL,
        bool hide_text_after_double_hash = false,
        float wrap_width = -1.0f) override;
    ImVec4 ColorConvertU32ToFloat4(ImU32 in) override;
    ImU32 ColorConvertFloat4ToU32(const ImVec4& in) override;
    void ColorConvertRGBtoHSV(float r,
        float g,
        float b,
        float& out_h,
        float& out_s,
        float& out_v) override;
    void ColorConvertHSVtoRGB(float h,
        float s,
        float v,
        float& out_r,
        float& out_g,
        float& out_b) override;
    bool IsKeyDown(ImGuiKey key) override;
    bool IsKeyPressed(ImGuiKey key, bool repeat = true) override;
    bool IsKeyReleased(ImGuiKey key) override;
    bool IsKeyChordPressed(ImGuiKeyChord key_chord) override;
    int GetKeyPressedAmount(
        ImGuiKey key, float repeat_delay, float rate) override;
    const char* GetKeyName(ImGuiKey key) override;
    void SetNextFrameWantCaptureKeyboard(bool want_capture_keyboard) override;
    bool Shortcut(ImGuiKeyChord key_chord, ImGuiInputFlags flags = 0) override;
    void SetNextItemShortcut(
        ImGuiKeyChord key_chord, ImGuiInputFlags flags = 0) override;
    bool SetItemKeyOwner(ImGuiKey key) override;
    bool IsMouseDown(ImGuiMouseButton button) override;
    bool IsMouseClicked(ImGuiMouseButton button, bool repeat = false) override;
    bool IsMouseReleased(ImGuiMouseButton button) override;
    bool IsMouseDoubleClicked(ImGuiMouseButton button) override;
    bool IsMouseReleasedWithDelay(
        ImGuiMouseButton button, float delay = -1.f) override;
    int GetMouseClickedCount(ImGuiMouseButton button) override;
    bool IsMouseHoveringRect(
        const ImVec2& r_min, const ImVec2& r_max, bool clip = true) override;
    bool IsMousePosValid(const ImVec2* mouse_pos = NULL) override;
    bool IsAnyMouseDown() override;
    ImVec2 GetMousePos() override;
    ImVec2 GetMousePosOnOpeningCurrentPopup() override;
    bool IsMouseDragging(
        ImGuiMouseButton button, float lock_threshold = -1.0f) override;
    ImVec2 GetMouseDragDelta(
        ImGuiMouseButton button = 0, float lock_threshold = -1.0f) override;
    void ResetMouseDragDelta(ImGuiMouseButton button = 0) override;
    ImGuiMouseCursor GetMouseCursor() override;
    void SetMouseCursor(ImGuiMouseCursor cursor_type) override;
    void SetNextFrameWantCaptureMouse(bool want_capture_mouse) override;
    const char* GetClipboardText() override;
    void SetClipboardText(const char* text) override;
    void LoadIniSettingsFromDisk(const char* ini_filename) override;
    void LoadIniSettingsFromMemory(
        const char* ini_data, size_t ini_size = 0) override;
    void SaveIniSettingsToDisk(const char* ini_filename) override;
    const char* SaveIniSettingsToMemory(size_t* out_ini_size = NULL) override;
    void DebugTextEncoding(const char* text) override;
    void DebugFlashStyleColor(ImGuiCol idx) override;
    void DebugStartItemPicker() override;
    bool DebugCheckVersionAndDataLayout(const char* version_str,
        size_t sz_io,
        size_t sz_style,
        size_t sz_vec2,
        size_t sz_vec4,
        size_t sz_drawvert,
        size_t sz_drawidx) override;
    void DebugLog(const char* fmt, ...) override;
    void DebugLogV(const char* fmt, va_list args) override;
    void SetAllocatorFunctions(ImGuiMemAllocFunc alloc_func,
        ImGuiMemFreeFunc free_func,
        void* user_data = NULL) override;
    void GetAllocatorFunctions(ImGuiMemAllocFunc* p_alloc_func,
        ImGuiMemFreeFunc* p_free_func,
        void** p_user_data) override;
    void* MemAlloc(size_t size) override;
    void MemFree(void* ptr) override;
    void UpdatePlatformWindows() override;
    void RenderPlatformWindowsDefault(void* platform_render_arg = NULL,
        void* renderer_render_arg = NULL) override;
    void DestroyPlatformWindows() override;
    ImGuiViewport* FindViewportByID(ImGuiID viewport_id) override;
    ImGuiViewport* FindViewportByPlatformHandle(void* platform_handle) override;
    ImPlotContext* ImPlot_CreateContext() override;
    void ImPlot_DestroyContext(ImPlotContext* ctx = nullptr) override;
    ImPlotContext* ImPlot_GetCurrentContext() override;
    void ImPlot_SetCurrentContext(ImPlotContext* ctx) override;
    void SetImGuiContext(ImGuiContext* ctx) override;
    bool BeginPlot(const char* title_id,
        const ImVec2& size = ImVec2(-1, 0),
        ImPlotFlags flags = 0) override;
    void EndPlot() override;
    bool BeginSubplots(const char* title_id,
        int rows,
        int cols,
        const ImVec2& size,
        ImPlotSubplotFlags flags = 0,
        float* row_ratios = nullptr,
        float* col_ratios = nullptr) override;
    void EndSubplots() override;
    void SetupAxis(ImAxis axis,
        const char* label = nullptr,
        ImPlotAxisFlags flags = 0) override;
    void SetupAxisLimits(ImAxis axis,
        double v_min,
        double v_max,
        ImPlotCond cond = ImPlotCond_Once) override;
    void SetupAxisLinks(
        ImAxis axis, double* link_min, double* link_max) override;
    void SetupAxisFormat(ImAxis axis, const char* fmt) override;
    void SetupAxisFormat(
        ImAxis axis, ImPlotFormatter formatter, void* data = nullptr) override;
    void SetupAxisTicks(ImAxis axis,
        const double* values,
        int n_ticks,
        const char* const labels[] = nullptr,
        bool keep_default = false) override;
    void SetupAxisTicks(ImAxis axis,
        double v_min,
        double v_max,
        int n_ticks,
        const char* const labels[] = nullptr,
        bool keep_default = false) override;
    void SetupAxisScale(ImAxis axis, ImPlotScale scale) override;
    void SetupAxisScale(ImAxis axis,
        ImPlotTransform forward,
        ImPlotTransform inverse,
        void* data = nullptr) override;
    void SetupAxisLimitsConstraints(
        ImAxis axis, double v_min, double v_max) override;
    void SetupAxisZoomConstraints(
        ImAxis axis, double z_min, double z_max) override;
    void SetupAxes(const char* x_label,
        const char* y_label,
        ImPlotAxisFlags x_flags = 0,
        ImPlotAxisFlags y_flags = 0) override;
    void SetupAxesLimits(double x_min,
        double x_max,
        double y_min,
        double y_max,
        ImPlotCond cond = ImPlotCond_Once) override;
    void SetupLegend(
        ImPlotLocation location, ImPlotLegendFlags flags = 0) override;
    void SetupMouseText(
        ImPlotLocation location, ImPlotMouseTextFlags flags = 0) override;
    void SetupFinish() override;
    void SetNextAxisLimits(ImAxis axis,
        double v_min,
        double v_max,
        ImPlotCond cond = ImPlotCond_Once) override;
    void SetNextAxisLinks(
        ImAxis axis, double* link_min, double* link_max) override;
    void SetNextAxisToFit(ImAxis axis) override;
    void SetNextAxesLimits(double x_min,
        double x_max,
        double y_min,
        double y_max,
        ImPlotCond cond = ImPlotCond_Once) override;
    void SetNextAxesToFit() override;
    void PlotLineG(const char* label_id,
        ImPlotGetter getter,
        void* data,
        int count,
        const ImPlotSpec& spec = ImPlotSpec()) override;
    void PlotScatterG(const char* label_id,
        ImPlotGetter getter,
        void* data,
        int count,
        const ImPlotSpec& spec = ImPlotSpec()) override;
    void PlotStairsG(const char* label_id,
        ImPlotGetter getter,
        void* data,
        int count,
        const ImPlotSpec& spec = ImPlotSpec()) override;
    void PlotShadedG(const char* label_id,
        ImPlotGetter getter1,
        void* data1,
        ImPlotGetter getter2,
        void* data2,
        int count,
        const ImPlotSpec& spec = ImPlotSpec()) override;
    void PlotBarsG(const char* label_id,
        ImPlotGetter getter,
        void* data,
        int count,
        double bar_size,
        const ImPlotSpec& spec = ImPlotSpec()) override;
    void PlotDigitalG(const char* label_id,
        ImPlotGetter getter,
        void* data,
        int count,
        const ImPlotSpec& spec = ImPlotSpec()) override;
    void PlotImage(const char* label_id,
        ImTextureRef tex_ref,
        const ImPlotPoint& bounds_min,
        const ImPlotPoint& bounds_max,
        const ImVec2& uv0 = ImVec2(0, 0),
        const ImVec2& uv1 = ImVec2(1, 1),
        const ImVec4& tint_col = ImVec4(1, 1, 1, 1),
        const ImPlotSpec& spec = ImPlotSpec()) override;
    void PlotImage(const char* label_id,
        ImTextureID tex_ref,
        const ImPlotPoint& bounds_min,
        const ImPlotPoint& bounds_max,
        const ImVec2& uv0 = ImVec2(0, 0),
        const ImVec2& uv1 = ImVec2(1, 1),
        const ImVec4& tint_col = ImVec4(1, 1, 1, 1),
        const ImPlotSpec& spec = ImPlotSpec()) override;
    void PlotText(const char* text,
        double x,
        double y,
        const ImVec2& pix_offset = ImVec2(0, 0),
        const ImPlotSpec& spec = ImPlotSpec()) override;
    void PlotDummy(
        const char* label_id, const ImPlotSpec& spec = ImPlotSpec()) override;
    bool DragPoint(int id,
        double* x,
        double* y,
        const ImVec4& col,
        float size = 4,
        ImPlotDragToolFlags flags = 0,
        bool* out_clicked = nullptr,
        bool* out_hovered = nullptr,
        bool* out_held = nullptr) override;
    bool DragLineX(int id,
        double* x,
        const ImVec4& col,
        float thickness = 1,
        ImPlotDragToolFlags flags = 0,
        bool* out_clicked = nullptr,
        bool* out_hovered = nullptr,
        bool* out_held = nullptr) override;
    bool DragLineY(int id,
        double* y,
        const ImVec4& col,
        float thickness = 1,
        ImPlotDragToolFlags flags = 0,
        bool* out_clicked = nullptr,
        bool* out_hovered = nullptr,
        bool* out_held = nullptr) override;
    bool DragRect(int id,
        double* x1,
        double* y1,
        double* x2,
        double* y2,
        const ImVec4& col,
        ImPlotDragToolFlags flags = 0,
        bool* out_clicked = nullptr,
        bool* out_hovered = nullptr,
        bool* out_held = nullptr) override;
    void Annotation(double x,
        double y,
        const ImVec4& col,
        const ImVec2& pix_offset,
        bool clamp,
        bool round = false) override;
    void Annotation(double x,
        double y,
        const ImVec4& col,
        const ImVec2& pix_offset,
        bool clamp,
        const char* fmt,
        ...) override;
    void AnnotationV(double x,
        double y,
        const ImVec4& col,
        const ImVec2& pix_offset,
        bool clamp,
        const char* fmt,
        va_list args) override;
    void TagX(double x, const ImVec4& col, bool round = false) override;
    void TagX(double x, const ImVec4& col, const char* fmt, ...) override;
    void TagXV(
        double x, const ImVec4& col, const char* fmt, va_list args) override;
    void TagY(double y, const ImVec4& col, bool round = false) override;
    void TagY(double y, const ImVec4& col, const char* fmt, ...) override;
    void TagYV(
        double y, const ImVec4& col, const char* fmt, va_list args) override;
    void SetAxis(ImAxis axis) override;
    void SetAxes(ImAxis x_axis, ImAxis y_axis) override;
    ImPlotPoint PixelsToPlot(const ImVec2& pix,
        ImAxis x_axis = IMPLOT_AUTO,
        ImAxis y_axis = IMPLOT_AUTO) override;
    ImPlotPoint PixelsToPlot(float x,
        float y,
        ImAxis x_axis = IMPLOT_AUTO,
        ImAxis y_axis = IMPLOT_AUTO) override;
    ImVec2 PlotToPixels(const ImPlotPoint& plt,
        ImAxis x_axis = IMPLOT_AUTO,
        ImAxis y_axis = IMPLOT_AUTO) override;
    ImVec2 PlotToPixels(double x,
        double y,
        ImAxis x_axis = IMPLOT_AUTO,
        ImAxis y_axis = IMPLOT_AUTO) override;
    ImVec2 GetPlotPos() override;
    ImVec2 GetPlotSize() override;
    ImPlotPoint GetPlotMousePos(
        ImAxis x_axis = IMPLOT_AUTO, ImAxis y_axis = IMPLOT_AUTO) override;
    ImPlotRect GetPlotLimits(
        ImAxis x_axis = IMPLOT_AUTO, ImAxis y_axis = IMPLOT_AUTO) override;
    bool IsPlotHovered() override;
    bool IsAxisHovered(ImAxis axis) override;
    bool IsSubplotsHovered() override;
    bool IsPlotSelected() override;
    ImPlotRect GetPlotSelection(
        ImAxis x_axis = IMPLOT_AUTO, ImAxis y_axis = IMPLOT_AUTO) override;
    void CancelPlotSelection() override;
    void HideNextItem(
        bool hidden = true, ImPlotCond cond = ImPlotCond_Once) override;
    bool BeginAlignedPlots(const char* group_id, bool vertical = true) override;
    void EndAlignedPlots() override;
    bool BeginLegendPopup(
        const char* label_id, ImGuiMouseButton mouse_button = 1) override;
    void EndLegendPopup() override;
    bool IsLegendEntryHovered(const char* label_id) override;
    bool BeginDragDropTargetPlot() override;
    bool BeginDragDropTargetAxis(ImAxis axis) override;
    bool BeginDragDropTargetLegend() override;
    void ImPlot_EndDragDropTarget() override;
    bool BeginDragDropSourcePlot(ImGuiDragDropFlags flags = 0) override;
    bool BeginDragDropSourceAxis(
        ImAxis axis, ImGuiDragDropFlags flags = 0) override;
    bool BeginDragDropSourceItem(
        const char* label_id, ImGuiDragDropFlags flags = 0) override;
    void ImPlot_EndDragDropSource() override;
    ImPlotStyle& ImPlot_GetStyle() override;
    void StyleColorsAuto(ImPlotStyle* dst = nullptr) override;
    void ImPlot_StyleColorsClassic(ImPlotStyle* dst = nullptr) override;
    void ImPlot_StyleColorsDark(ImPlotStyle* dst = nullptr) override;
    void ImPlot_StyleColorsLight(ImPlotStyle* dst = nullptr) override;
    void ImPlot_PushStyleColor(ImPlotCol idx, ImU32 col) override;
    void ImPlot_PushStyleColor(ImPlotCol idx, const ImVec4& col) override;
    void ImPlot_PopStyleColor(int count = 1) override;
    void ImPlot_PushStyleVar(ImPlotStyleVar idx, float val) override;
    void ImPlot_PushStyleVar(ImPlotStyleVar idx, int val) override;
    void ImPlot_PushStyleVar(ImPlotStyleVar idx, const ImVec2& val) override;
    void ImPlot_PopStyleVar(int count = 1) override;
    ImVec4 GetLastItemColor() override;
    const char* ImPlot_GetStyleColorName(ImPlotCol idx) override;
    const char* GetMarkerName(ImPlotMarker idx) override;
    ImPlotMarker NextMarker() override;
    ImPlotColormap AddColormap(const char* name,
        const ImVec4* cols,
        int size,
        bool qual = true) override;
    ImPlotColormap AddColormap(const char* name,
        const ImU32* cols,
        int size,
        bool qual = true) override;
    int GetColormapCount() override;
    const char* GetColormapName(ImPlotColormap cmap) override;
    ImPlotColormap GetColormapIndex(const char* name) override;
    void PushColormap(ImPlotColormap cmap) override;
    void PushColormap(const char* name) override;
    void PopColormap(int count = 1) override;
    ImVec4 NextColormapColor() override;
    int GetColormapSize(ImPlotColormap cmap = IMPLOT_AUTO) override;
    ImVec4 GetColormapColor(
        int idx, ImPlotColormap cmap = IMPLOT_AUTO) override;
    ImVec4 SampleColormap(float t, ImPlotColormap cmap = IMPLOT_AUTO) override;
    void ColormapScale(const char* label,
        double scale_min,
        double scale_max,
        const ImVec2& size = ImVec2(0, 0),
        const char* format = "%g",
        ImPlotColormapScaleFlags flags = 0,
        ImPlotColormap cmap = IMPLOT_AUTO) override;
    bool ColormapSlider(const char* label,
        float* t,
        ImVec4* out = nullptr,
        const char* format = "",
        ImPlotColormap cmap = IMPLOT_AUTO) override;
    bool ColormapButton(const char* label,
        const ImVec2& size = ImVec2(0, 0),
        ImPlotColormap cmap = IMPLOT_AUTO) override;
    void BustColorCache(const char* plot_title_id = nullptr) override;
    ImPlotInputMap& GetInputMap() override;
    void MapInputDefault(ImPlotInputMap* dst = nullptr) override;
    void MapInputReverse(ImPlotInputMap* dst = nullptr) override;
    void ItemIcon(const ImVec4& col) override;
    void ItemIcon(ImU32 col) override;
    void ColormapIcon(ImPlotColormap cmap) override;
    ImDrawList* GetPlotDrawList() override;
    void PushPlotClipRect(float expand = 0) override;
    void PopPlotClipRect() override;
    bool ImPlot_ShowStyleSelector(const char* label) override;
    bool ShowColormapSelector(const char* label) override;
    bool ShowInputMapSelector(const char* label) override;
    void ImPlot_ShowStyleEditor(ImPlotStyle* ref = nullptr) override;
    void ImPlot_ShowUserGuide() override;
    void ImPlot_ShowMetricsWindow(bool* p_popen = nullptr) override;
    void ImPlot_ShowDemoWindow(bool* p_open = nullptr) override;
    void SetColorEditOptions(ImGuiColorEditFlags flags) override;
    void SetWindowFontScale(float scale) override;
    void Image(ImTextureRef tex_ref,
        const ImVec2& image_size,
        const ImVec2& uv0,
        const ImVec2& uv1,
        const ImVec4& tint_col,
        const ImVec4& border_col) override;
    ImVec2 GetContentRegionMax() override;
    ImVec2 GetWindowContentRegionMin() override;
    ImVec2 GetWindowContentRegionMax() override;

protected:
    ImGuiIO _io{};
    ImGuiPlatformIO _platformIO{};
    ImGuiStyle _style{};
    ImPlotStyle _plotStyle{};
    ImVec4 _dummyVec4{1, 1, 1, 1};
    ImPlotInputMap _inputMap{};
    ImDrawListSharedData* _dummyDrawListShared = nullptr;
    ImDrawList* _dummyDrawList = nullptr;
    ImFont* _font = nullptr;
};