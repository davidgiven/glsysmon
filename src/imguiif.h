#pragma once

#include <cstdarg>
#include <memory>
#include <imgui.h>
#include <implot.h>

class ImGuiIf
{
public:
    virtual ~ImGuiIf() = default;

    virtual ImGuiContext* CreateContext(
        ImFontAtlas* shared_font_atlas = NULL) = 0;
    virtual void DestroyContext(ImGuiContext* ctx = NULL) = 0;
    virtual ImGuiContext* GetCurrentContext() = 0;
    virtual void SetCurrentContext(ImGuiContext* ctx) = 0;
    virtual ImGuiIO& GetIO() = 0;
    virtual ImGuiPlatformIO& GetPlatformIO() = 0;
    virtual ImGuiStyle& GetStyle() = 0;
    virtual void NewFrame() = 0;
    virtual void EndFrame() = 0;
    virtual void Render() = 0;
    virtual ImDrawData* GetDrawData() = 0;
    virtual void ShowDemoWindow(bool* p_open = NULL) = 0;
    virtual void ShowMetricsWindow(bool* p_open = NULL) = 0;
    virtual void ShowDebugLogWindow(bool* p_open = NULL) = 0;
    virtual void ShowIDStackToolWindow(bool* p_open = NULL) = 0;
    virtual void ShowAboutWindow(bool* p_open = NULL) = 0;
    virtual void ShowStyleEditor(ImGuiStyle* ref = NULL) = 0;
    virtual bool ShowStyleSelector(const char* label) = 0;
    virtual void ShowFontSelector(const char* label) = 0;
    virtual void ShowUserGuide() = 0;
    virtual const char* GetVersion() = 0;
    virtual void StyleColorsDark(ImGuiStyle* dst = NULL) = 0;
    virtual void StyleColorsLight(ImGuiStyle* dst = NULL) = 0;
    virtual void StyleColorsClassic(ImGuiStyle* dst = NULL) = 0;
    virtual bool Begin(
        const char* name, bool* p_open = NULL, ImGuiWindowFlags flags = 0) = 0;
    virtual void End() = 0;
    virtual bool BeginChild(const char* str_id,
        const ImVec2& size = ImVec2(0, 0),
        ImGuiChildFlags child_flags = 0,
        ImGuiWindowFlags window_flags = 0) = 0;
    virtual bool BeginChild(ImGuiID id,
        const ImVec2& size = ImVec2(0, 0),
        ImGuiChildFlags child_flags = 0,
        ImGuiWindowFlags window_flags = 0) = 0;
    virtual void EndChild() = 0;
    virtual bool IsWindowAppearing() = 0;
    virtual bool IsWindowCollapsed() = 0;
    virtual bool IsWindowFocused(ImGuiFocusedFlags flags = 0) = 0;
    virtual bool IsWindowHovered(ImGuiHoveredFlags flags = 0) = 0;
    virtual ImDrawList* GetWindowDrawList() = 0;
    virtual float GetWindowDpiScale() = 0;
    virtual ImVec2 GetWindowPos() = 0;
    virtual ImVec2 GetWindowSize() = 0;
    virtual float GetWindowWidth() = 0;
    virtual float GetWindowHeight() = 0;
    virtual ImGuiViewport* GetWindowViewport() = 0;
    virtual void SetNextWindowPos(const ImVec2& pos,
        ImGuiCond cond = 0,
        const ImVec2& pivot = ImVec2(0, 0)) = 0;
    virtual void SetNextWindowSize(const ImVec2& size, ImGuiCond cond = 0) = 0;
    virtual void SetNextWindowSizeConstraints(const ImVec2& size_min,
        const ImVec2& size_max,
        ImGuiSizeCallback custom_callback = NULL,
        void* custom_callback_data = NULL) = 0;
    virtual void SetNextWindowContentSize(const ImVec2& size) = 0;
    virtual void SetNextWindowCollapsed(bool collapsed, ImGuiCond cond = 0) = 0;
    virtual void SetNextWindowFocus() = 0;
    virtual void SetNextWindowScroll(const ImVec2& scroll) = 0;
    virtual void SetNextWindowBgAlpha(float alpha) = 0;
    virtual void SetNextWindowViewport(ImGuiID viewport_id) = 0;
    virtual void SetWindowPos(const ImVec2& pos, ImGuiCond cond = 0) = 0;
    virtual void SetWindowSize(const ImVec2& size, ImGuiCond cond = 0) = 0;
    virtual void SetWindowCollapsed(bool collapsed, ImGuiCond cond = 0) = 0;
    virtual void SetWindowFocus() = 0;
    virtual void SetWindowPos(
        const char* name, const ImVec2& pos, ImGuiCond cond = 0) = 0;
    virtual void SetWindowSize(
        const char* name, const ImVec2& size, ImGuiCond cond = 0) = 0;
    virtual void SetWindowCollapsed(
        const char* name, bool collapsed, ImGuiCond cond = 0) = 0;
    virtual void SetWindowFocus(const char* name) = 0;
    virtual float GetScrollX() = 0;
    virtual float GetScrollY() = 0;
    virtual void SetScrollX(float scroll_x) = 0;
    virtual void SetScrollY(float scroll_y) = 0;
    virtual float GetScrollMaxX() = 0;
    virtual float GetScrollMaxY() = 0;
    virtual void SetScrollHereX(float center_x_ratio = 0.5f) = 0;
    virtual void SetScrollHereY(float center_y_ratio = 0.5f) = 0;
    virtual void SetScrollFromPosX(
        float local_x, float center_x_ratio = 0.5f) = 0;
    virtual void SetScrollFromPosY(
        float local_y, float center_y_ratio = 0.5f) = 0;
    virtual void PushFont(ImFont* font, float font_size_base_unscaled) = 0;
    virtual void PopFont() = 0;
    virtual ImFont* GetFont() = 0;
    virtual float GetFontSize() = 0;
    virtual ImFontBaked* GetFontBaked() = 0;
    virtual void PushStyleColor(ImGuiCol idx, ImU32 col) = 0;
    virtual void PushStyleColor(ImGuiCol idx, const ImVec4& col) = 0;
    virtual void PopStyleColor(int count = 1) = 0;
    virtual void PushStyleVar(ImGuiStyleVar idx, float val) = 0;
    virtual void PushStyleVar(ImGuiStyleVar idx, const ImVec2& val) = 0;
    virtual void PushStyleVarX(ImGuiStyleVar idx, float val_x) = 0;
    virtual void PushStyleVarY(ImGuiStyleVar idx, float val_y) = 0;
    virtual void PopStyleVar(int count = 1) = 0;
    virtual void PushItemFlag(ImGuiItemFlags option, bool enabled) = 0;
    virtual void PopItemFlag() = 0;
    virtual void PushItemWidth(float item_width) = 0;
    virtual void PopItemWidth() = 0;
    virtual void SetNextItemWidth(float item_width) = 0;
    virtual float CalcItemWidth() = 0;
    virtual void PushTextWrapPos(float wrap_local_pos_x = 0.0f) = 0;
    virtual void PopTextWrapPos() = 0;
    virtual ImVec2 GetFontTexUvWhitePixel() = 0;
    virtual ImU32 GetColorU32(ImGuiCol idx, float alpha_mul = 1.0f) = 0;
    virtual ImU32 GetColorU32(const ImVec4& col) = 0;
    virtual ImU32 GetColorU32(ImU32 col, float alpha_mul = 1.0f) = 0;
    virtual const ImVec4& GetStyleColorVec4(ImGuiCol idx) = 0;
    virtual ImVec2 GetCursorScreenPos() = 0;
    virtual void SetCursorScreenPos(const ImVec2& pos) = 0;
    virtual ImVec2 GetContentRegionAvail() = 0;
    virtual ImVec2 GetCursorPos() = 0;
    virtual float GetCursorPosX() = 0;
    virtual float GetCursorPosY() = 0;
    virtual void SetCursorPos(const ImVec2& local_pos) = 0;
    virtual void SetCursorPosX(float local_x) = 0;
    virtual void SetCursorPosY(float local_y) = 0;
    virtual ImVec2 GetCursorStartPos() = 0;
    virtual void Separator() = 0;
    virtual void SameLine(
        float offset_from_start_x = 0.0f, float spacing = -1.0f) = 0;
    virtual void NewLine() = 0;
    virtual void Spacing() = 0;
    virtual void Dummy(const ImVec2& size) = 0;
    virtual void Indent(float indent_w = 0.0f) = 0;
    virtual void Unindent(float indent_w = 0.0f) = 0;
    virtual void BeginGroup() = 0;
    virtual void EndGroup() = 0;
    virtual void AlignTextToFramePadding() = 0;
    virtual float GetTextLineHeight() = 0;
    virtual float GetTextLineHeightWithSpacing() = 0;
    virtual float GetFrameHeight() = 0;
    virtual float GetFrameHeightWithSpacing() = 0;
    virtual void PushID(const char* str_id) = 0;
    virtual void PushID(const char* str_id_begin, const char* str_id_end) = 0;
    virtual void PushID(const void* ptr_id) = 0;
    virtual void PushID(int int_id) = 0;
    virtual void PopID() = 0;
    virtual ImGuiID GetID(const char* str_id) = 0;
    virtual ImGuiID GetID(const char* str_id_begin, const char* str_id_end) = 0;
    virtual ImGuiID GetID(const void* ptr_id) = 0;
    virtual ImGuiID GetID(int int_id) = 0;
    virtual void TextUnformatted(
        const char* text, const char* text_end = NULL) = 0;
    virtual void Text(const char* fmt, ...) = 0;
    virtual void TextV(const char* fmt, va_list args) = 0;
    virtual void TextColored(const ImVec4& col, const char* fmt, ...) = 0;
    virtual void TextColoredV(
        const ImVec4& col, const char* fmt, va_list args) = 0;
    virtual void TextDisabled(const char* fmt, ...) = 0;
    virtual void TextDisabledV(const char* fmt, va_list args) = 0;
    virtual void TextWrapped(const char* fmt, ...) = 0;
    virtual void TextWrappedV(const char* fmt, va_list args) = 0;
    virtual void LabelText(const char* label, const char* fmt, ...) = 0;
    virtual void LabelTextV(
        const char* label, const char* fmt, va_list args) = 0;
    virtual void BulletText(const char* fmt, ...) = 0;
    virtual void BulletTextV(const char* fmt, va_list args) = 0;
    virtual void SeparatorText(const char* label) = 0;
    virtual bool Button(
        const char* label, const ImVec2& size = ImVec2(0, 0)) = 0;
    virtual bool SmallButton(const char* label) = 0;
    virtual bool InvisibleButton(
        const char* str_id, const ImVec2& size, ImGuiButtonFlags flags = 0) = 0;
    virtual bool ArrowButton(const char* str_id, ImGuiDir dir) = 0;
    virtual bool Checkbox(const char* label, bool* v) = 0;
    virtual bool CheckboxFlags(
        const char* label, int* flags, int flags_value) = 0;
    virtual bool CheckboxFlags(
        const char* label, unsigned int* flags, unsigned int flags_value) = 0;
    virtual bool RadioButton(const char* label, bool active) = 0;
    virtual bool RadioButton(const char* label, int* v, int v_button) = 0;
    virtual void ProgressBar(float fraction,
        const ImVec2& size_arg = ImVec2(-FLT_MIN, 0),
        const char* overlay = NULL) = 0;
    virtual void Bullet() = 0;
    virtual bool TextLink(const char* label) = 0;
    virtual bool TextLinkOpenURL(const char* label, const char* url = NULL) = 0;
    virtual void Image(ImTextureRef tex_ref,
        const ImVec2& image_size,
        const ImVec2& uv0 = ImVec2(0, 0),
        const ImVec2& uv1 = ImVec2(1, 1)) = 0;
    virtual void ImageWithBg(ImTextureRef tex_ref,
        const ImVec2& image_size,
        const ImVec2& uv0 = ImVec2(0, 0),
        const ImVec2& uv1 = ImVec2(1, 1),
        const ImVec4& bg_col = ImVec4(0, 0, 0, 0),
        const ImVec4& tint_col = ImVec4(1, 1, 1, 1)) = 0;
    virtual bool ImageButton(const char* str_id,
        ImTextureRef tex_ref,
        const ImVec2& image_size,
        const ImVec2& uv0 = ImVec2(0, 0),
        const ImVec2& uv1 = ImVec2(1, 1),
        const ImVec4& bg_col = ImVec4(0, 0, 0, 0),
        const ImVec4& tint_col = ImVec4(1, 1, 1, 1)) = 0;
    virtual bool BeginCombo(const char* label,
        const char* preview_value,
        ImGuiComboFlags flags = 0) = 0;
    virtual void EndCombo() = 0;
    virtual bool Combo(const char* label,
        int* current_item,
        const char* const items[],
        int items_count,
        int popup_max_height_in_items = -1) = 0;
    virtual bool Combo(const char* label,
        int* current_item,
        const char* items_separated_by_zeros,
        int popup_max_height_in_items = -1) = 0;
    virtual bool Combo(const char* label,
        int* current_item,
        const char* (*getter)(void* user_data, int idx),
        void* user_data,
        int items_count,
        int popup_max_height_in_items = -1) = 0;
    virtual bool DragFloat(const char* label,
        float* v,
        float v_speed = 1.0f,
        float v_min = 0.0f,
        float v_max = 0.0f,
        const char* format = "%.3f",
        ImGuiSliderFlags flags = 0) = 0;
    virtual bool DragFloat2(const char* label,
        float v[2],
        float v_speed = 1.0f,
        float v_min = 0.0f,
        float v_max = 0.0f,
        const char* format = "%.3f",
        ImGuiSliderFlags flags = 0) = 0;
    virtual bool DragFloat3(const char* label,
        float v[3],
        float v_speed = 1.0f,
        float v_min = 0.0f,
        float v_max = 0.0f,
        const char* format = "%.3f",
        ImGuiSliderFlags flags = 0) = 0;
    virtual bool DragFloat4(const char* label,
        float v[4],
        float v_speed = 1.0f,
        float v_min = 0.0f,
        float v_max = 0.0f,
        const char* format = "%.3f",
        ImGuiSliderFlags flags = 0) = 0;
    virtual bool DragFloatRange2(const char* label,
        float* v_current_min,
        float* v_current_max,
        float v_speed = 1.0f,
        float v_min = 0.0f,
        float v_max = 0.0f,
        const char* format = "%.3f",
        const char* format_max = NULL,
        ImGuiSliderFlags flags = 0) = 0;
    virtual bool DragInt(const char* label,
        int* v,
        float v_speed = 1.0f,
        int v_min = 0,
        int v_max = 0,
        const char* format = "%d",
        ImGuiSliderFlags flags = 0) = 0;
    virtual bool DragInt2(const char* label,
        int v[2],
        float v_speed = 1.0f,
        int v_min = 0,
        int v_max = 0,
        const char* format = "%d",
        ImGuiSliderFlags flags = 0) = 0;
    virtual bool DragInt3(const char* label,
        int v[3],
        float v_speed = 1.0f,
        int v_min = 0,
        int v_max = 0,
        const char* format = "%d",
        ImGuiSliderFlags flags = 0) = 0;
    virtual bool DragInt4(const char* label,
        int v[4],
        float v_speed = 1.0f,
        int v_min = 0,
        int v_max = 0,
        const char* format = "%d",
        ImGuiSliderFlags flags = 0) = 0;
    virtual bool DragIntRange2(const char* label,
        int* v_current_min,
        int* v_current_max,
        float v_speed = 1.0f,
        int v_min = 0,
        int v_max = 0,
        const char* format = "%d",
        const char* format_max = NULL,
        ImGuiSliderFlags flags = 0) = 0;
    virtual bool DragScalar(const char* label,
        ImGuiDataType data_type,
        void* p_data,
        float v_speed = 1.0f,
        const void* p_min = NULL,
        const void* p_max = NULL,
        const char* format = NULL,
        ImGuiSliderFlags flags = 0) = 0;
    virtual bool DragScalarN(const char* label,
        ImGuiDataType data_type,
        void* p_data,
        int components,
        float v_speed = 1.0f,
        const void* p_min = NULL,
        const void* p_max = NULL,
        const char* format = NULL,
        ImGuiSliderFlags flags = 0) = 0;
    virtual bool SliderFloat(const char* label,
        float* v,
        float v_min,
        float v_max,
        const char* format = "%.3f",
        ImGuiSliderFlags flags = 0) = 0;
    virtual bool SliderFloat2(const char* label,
        float v[2],
        float v_min,
        float v_max,
        const char* format = "%.3f",
        ImGuiSliderFlags flags = 0) = 0;
    virtual bool SliderFloat3(const char* label,
        float v[3],
        float v_min,
        float v_max,
        const char* format = "%.3f",
        ImGuiSliderFlags flags = 0) = 0;
    virtual bool SliderFloat4(const char* label,
        float v[4],
        float v_min,
        float v_max,
        const char* format = "%.3f",
        ImGuiSliderFlags flags = 0) = 0;
    virtual bool SliderAngle(const char* label,
        float* v_rad,
        float v_degrees_min = -360.0f,
        float v_degrees_max = +360.0f,
        const char* format = "%.0f deg",
        ImGuiSliderFlags flags = 0) = 0;
    virtual bool SliderInt(const char* label,
        int* v,
        int v_min,
        int v_max,
        const char* format = "%d",
        ImGuiSliderFlags flags = 0) = 0;
    virtual bool SliderInt2(const char* label,
        int v[2],
        int v_min,
        int v_max,
        const char* format = "%d",
        ImGuiSliderFlags flags = 0) = 0;
    virtual bool SliderInt3(const char* label,
        int v[3],
        int v_min,
        int v_max,
        const char* format = "%d",
        ImGuiSliderFlags flags = 0) = 0;
    virtual bool SliderInt4(const char* label,
        int v[4],
        int v_min,
        int v_max,
        const char* format = "%d",
        ImGuiSliderFlags flags = 0) = 0;
    virtual bool SliderScalar(const char* label,
        ImGuiDataType data_type,
        void* p_data,
        const void* p_min,
        const void* p_max,
        const char* format = NULL,
        ImGuiSliderFlags flags = 0) = 0;
    virtual bool SliderScalarN(const char* label,
        ImGuiDataType data_type,
        void* p_data,
        int components,
        const void* p_min,
        const void* p_max,
        const char* format = NULL,
        ImGuiSliderFlags flags = 0) = 0;
    virtual bool VSliderFloat(const char* label,
        const ImVec2& size,
        float* v,
        float v_min,
        float v_max,
        const char* format = "%.3f",
        ImGuiSliderFlags flags = 0) = 0;
    virtual bool VSliderInt(const char* label,
        const ImVec2& size,
        int* v,
        int v_min,
        int v_max,
        const char* format = "%d",
        ImGuiSliderFlags flags = 0) = 0;
    virtual bool VSliderScalar(const char* label,
        const ImVec2& size,
        ImGuiDataType data_type,
        void* p_data,
        const void* p_min,
        const void* p_max,
        const char* format = NULL,
        ImGuiSliderFlags flags = 0) = 0;
    virtual bool InputText(const char* label,
        char* buf,
        size_t buf_size,
        ImGuiInputTextFlags flags = 0,
        ImGuiInputTextCallback callback = NULL,
        void* user_data = NULL) = 0;
    virtual bool InputTextMultiline(const char* label,
        char* buf,
        size_t buf_size,
        const ImVec2& size = ImVec2(0, 0),
        ImGuiInputTextFlags flags = 0,
        ImGuiInputTextCallback callback = NULL,
        void* user_data = NULL) = 0;
    virtual bool InputTextWithHint(const char* label,
        const char* hint,
        char* buf,
        size_t buf_size,
        ImGuiInputTextFlags flags = 0,
        ImGuiInputTextCallback callback = NULL,
        void* user_data = NULL) = 0;
    virtual bool InputFloat(const char* label,
        float* v,
        float step = 0.0f,
        float step_fast = 0.0f,
        const char* format = "%.3f",
        ImGuiInputTextFlags flags = 0) = 0;
    virtual bool InputFloat2(const char* label,
        float v[2],
        const char* format = "%.3f",
        ImGuiInputTextFlags flags = 0) = 0;
    virtual bool InputFloat3(const char* label,
        float v[3],
        const char* format = "%.3f",
        ImGuiInputTextFlags flags = 0) = 0;
    virtual bool InputFloat4(const char* label,
        float v[4],
        const char* format = "%.3f",
        ImGuiInputTextFlags flags = 0) = 0;
    virtual bool InputInt(const char* label,
        int* v,
        int step = 1,
        int step_fast = 100,
        ImGuiInputTextFlags flags = 0) = 0;
    virtual bool InputInt2(
        const char* label, int v[2], ImGuiInputTextFlags flags = 0) = 0;
    virtual bool InputInt3(
        const char* label, int v[3], ImGuiInputTextFlags flags = 0) = 0;
    virtual bool InputInt4(
        const char* label, int v[4], ImGuiInputTextFlags flags = 0) = 0;
    virtual bool InputDouble(const char* label,
        double* v,
        double step = 0.0,
        double step_fast = 0.0,
        const char* format = "%.6f",
        ImGuiInputTextFlags flags = 0) = 0;
    virtual bool InputScalar(const char* label,
        ImGuiDataType data_type,
        void* p_data,
        const void* p_step = NULL,
        const void* p_step_fast = NULL,
        const char* format = NULL,
        ImGuiInputTextFlags flags = 0) = 0;
    virtual bool InputScalarN(const char* label,
        ImGuiDataType data_type,
        void* p_data,
        int components,
        const void* p_step = NULL,
        const void* p_step_fast = NULL,
        const char* format = NULL,
        ImGuiInputTextFlags flags = 0) = 0;
    virtual bool ColorEdit3(
        const char* label, float col[3], ImGuiColorEditFlags flags = 0) = 0;
    virtual bool ColorEdit4(
        const char* label, float col[4], ImGuiColorEditFlags flags = 0) = 0;
    virtual bool ColorPicker3(
        const char* label, float col[3], ImGuiColorEditFlags flags = 0) = 0;
    virtual bool ColorPicker4(const char* label,
        float col[4],
        ImGuiColorEditFlags flags = 0,
        const float* ref_col = NULL) = 0;
    virtual bool ColorButton(const char* desc_id,
        const ImVec4& col,
        ImGuiColorEditFlags flags = 0,
        const ImVec2& size = ImVec2(0, 0)) = 0;
    virtual bool TreeNode(const char* label) = 0;
    virtual bool TreeNode(const char* str_id, const char* fmt, ...) = 0;
    virtual bool TreeNode(const void* ptr_id, const char* fmt, ...) = 0;
    virtual bool TreeNodeV(
        const char* str_id, const char* fmt, va_list args) = 0;
    virtual bool TreeNodeV(
        const void* ptr_id, const char* fmt, va_list args) = 0;
    virtual bool TreeNodeEx(
        const char* label, ImGuiTreeNodeFlags flags = 0) = 0;
    virtual bool TreeNodeEx(
        const char* str_id, ImGuiTreeNodeFlags flags, const char* fmt, ...) = 0;
    virtual bool TreeNodeEx(
        const void* ptr_id, ImGuiTreeNodeFlags flags, const char* fmt, ...) = 0;
    virtual bool TreeNodeExV(const char* str_id,
        ImGuiTreeNodeFlags flags,
        const char* fmt,
        va_list args) = 0;
    virtual bool TreeNodeExV(const void* ptr_id,
        ImGuiTreeNodeFlags flags,
        const char* fmt,
        va_list args) = 0;
    virtual void TreePush(const char* str_id) = 0;
    virtual void TreePush(const void* ptr_id) = 0;
    virtual void TreePop() = 0;
    virtual float GetTreeNodeToLabelSpacing() = 0;
    virtual bool CollapsingHeader(
        const char* label, ImGuiTreeNodeFlags flags = 0) = 0;
    virtual bool CollapsingHeader(
        const char* label, bool* p_visible, ImGuiTreeNodeFlags flags = 0) = 0;
    virtual void SetNextItemOpen(bool is_open, ImGuiCond cond = 0) = 0;
    virtual void SetNextItemStorageID(ImGuiID storage_id) = 0;
    virtual bool TreeNodeGetOpen(ImGuiID storage_id) = 0;
    virtual bool Selectable(const char* label,
        bool selected = false,
        ImGuiSelectableFlags flags = 0,
        const ImVec2& size = ImVec2(0, 0)) = 0;
    virtual bool Selectable(const char* label,
        bool* p_selected,
        ImGuiSelectableFlags flags = 0,
        const ImVec2& size = ImVec2(0, 0)) = 0;
    virtual ImGuiMultiSelectIO* BeginMultiSelect(ImGuiMultiSelectFlags flags,
        int selection_size = -1,
        int items_count = -1) = 0;
    virtual ImGuiMultiSelectIO* EndMultiSelect() = 0;
    virtual void SetNextItemSelectionUserData(
        ImGuiSelectionUserData selection_user_data) = 0;
    virtual bool IsItemToggledSelection() = 0;
    virtual bool BeginListBox(
        const char* label, const ImVec2& size = ImVec2(0, 0)) = 0;
    virtual void EndListBox() = 0;
    virtual bool ListBox(const char* label,
        int* current_item,
        const char* const items[],
        int items_count,
        int height_in_items = -1) = 0;
    virtual bool ListBox(const char* label,
        int* current_item,
        const char* (*getter)(void* user_data, int idx),
        void* user_data,
        int items_count,
        int height_in_items = -1) = 0;
    virtual void PlotLines(const char* label,
        const float* values,
        int values_count,
        int values_offset = 0,
        const char* overlay_text = NULL,
        float scale_min = FLT_MAX,
        float scale_max = FLT_MAX,
        ImVec2 graph_size = ImVec2(0, 0),
        int stride = sizeof(float)) = 0;
    virtual void PlotLines(const char* label,
        float (*values_getter)(void* data, int idx),
        void* data,
        int values_count,
        int values_offset = 0,
        const char* overlay_text = NULL,
        float scale_min = FLT_MAX,
        float scale_max = FLT_MAX,
        ImVec2 graph_size = ImVec2(0, 0)) = 0;
    virtual void PlotHistogram(const char* label,
        const float* values,
        int values_count,
        int values_offset = 0,
        const char* overlay_text = NULL,
        float scale_min = FLT_MAX,
        float scale_max = FLT_MAX,
        ImVec2 graph_size = ImVec2(0, 0),
        int stride = sizeof(float)) = 0;
    virtual void PlotHistogram(const char* label,
        float (*values_getter)(void* data, int idx),
        void* data,
        int values_count,
        int values_offset = 0,
        const char* overlay_text = NULL,
        float scale_min = FLT_MAX,
        float scale_max = FLT_MAX,
        ImVec2 graph_size = ImVec2(0, 0)) = 0;
    virtual void Value(const char* prefix, bool b) = 0;
    virtual void Value(const char* prefix, int v) = 0;
    virtual void Value(const char* prefix, unsigned int v) = 0;
    virtual void Value(
        const char* prefix, float v, const char* float_format = NULL) = 0;
    virtual bool BeginMenuBar() = 0;
    virtual void EndMenuBar() = 0;
    virtual bool BeginMainMenuBar() = 0;
    virtual void EndMainMenuBar() = 0;
    virtual bool BeginMenu(const char* label, bool enabled = true) = 0;
    virtual void EndMenu() = 0;
    virtual bool MenuItem(const char* label,
        const char* shortcut = NULL,
        bool selected = false,
        bool enabled = true) = 0;
    virtual bool MenuItem(const char* label,
        const char* shortcut,
        bool* p_selected,
        bool enabled = true) = 0;
    virtual bool BeginTooltip() = 0;
    virtual void EndTooltip() = 0;
    virtual void SetTooltip(const char* fmt, ...) = 0;
    virtual void SetTooltipV(const char* fmt, va_list args) = 0;
    virtual bool BeginItemTooltip() = 0;
    virtual void SetItemTooltip(const char* fmt, ...) = 0;
    virtual void SetItemTooltipV(const char* fmt, va_list args) = 0;
    virtual bool BeginPopup(const char* str_id, ImGuiWindowFlags flags = 0) = 0;
    virtual bool BeginPopupModal(
        const char* name, bool* p_open = NULL, ImGuiWindowFlags flags = 0) = 0;
    virtual void EndPopup() = 0;
    virtual bool OpenPopup(
        const char* str_id, ImGuiPopupFlags popup_flags = 0) = 0;
    virtual bool OpenPopup(ImGuiID id, ImGuiPopupFlags popup_flags = 0) = 0;
    virtual bool OpenPopupOnItemClick(
        const char* str_id = NULL, ImGuiPopupFlags popup_flags = 0) = 0;
    virtual void CloseCurrentPopup() = 0;
    virtual bool BeginPopupContextItem(
        const char* str_id = NULL, ImGuiPopupFlags popup_flags = 0) = 0;
    virtual bool BeginPopupContextWindow(
        const char* str_id = NULL, ImGuiPopupFlags popup_flags = 0) = 0;
    virtual bool BeginPopupContextVoid(
        const char* str_id = NULL, ImGuiPopupFlags popup_flags = 0) = 0;
    virtual bool IsPopupOpen(const char* str_id, ImGuiPopupFlags flags = 0) = 0;
    virtual bool BeginTable(const char* str_id,
        int columns,
        ImGuiTableFlags flags = 0,
        const ImVec2& outer_size = ImVec2(0.0f, 0.0f),
        float inner_width = 0.0f) = 0;
    virtual void EndTable() = 0;
    virtual void TableNextRow(
        ImGuiTableRowFlags row_flags = 0, float min_row_height = 0.0f) = 0;
    virtual bool TableNextColumn() = 0;
    virtual bool TableSetColumnIndex(int column_n) = 0;
    virtual void TableSetupColumn(const char* label,
        ImGuiTableColumnFlags flags = 0,
        float init_width_or_weight = 0.0f,
        ImGuiID user_data = 0) = 0;
    virtual void TableSetupScrollFreeze(int cols, int rows) = 0;
    virtual void TableHeader(const char* label) = 0;
    virtual void TableHeadersRow() = 0;
    virtual void TableAngledHeadersRow() = 0;
    virtual ImGuiTableSortSpecs* TableGetSortSpecs() = 0;
    virtual int TableGetColumnCount() = 0;
    virtual int TableGetColumnIndex() = 0;
    virtual int TableGetRowIndex() = 0;
    virtual const char* TableGetColumnName(int column_n = -1) = 0;
    virtual ImGuiTableColumnFlags TableGetColumnFlags(int column_n = -1) = 0;
    virtual void TableSetColumnEnabled(int column_n, bool v) = 0;
    virtual int TableGetHoveredColumn() = 0;
    virtual void TableSetBgColor(
        ImGuiTableBgTarget target, ImU32 color, int column_n = -1) = 0;
    virtual void Columns(
        int count = 1, const char* id = NULL, bool borders = true) = 0;
    virtual void NextColumn() = 0;
    virtual int GetColumnIndex() = 0;
    virtual float GetColumnWidth(int column_index = -1) = 0;
    virtual void SetColumnWidth(int column_index, float width) = 0;
    virtual float GetColumnOffset(int column_index = -1) = 0;
    virtual void SetColumnOffset(int column_index, float offset_x) = 0;
    virtual int GetColumnsCount() = 0;
    virtual bool BeginTabBar(
        const char* str_id, ImGuiTabBarFlags flags = 0) = 0;
    virtual void EndTabBar() = 0;
    virtual bool BeginTabItem(const char* label,
        bool* p_open = NULL,
        ImGuiTabItemFlags flags = 0) = 0;
    virtual void EndTabItem() = 0;
    virtual bool TabItemButton(
        const char* label, ImGuiTabItemFlags flags = 0) = 0;
    virtual void SetTabItemClosed(const char* tab_or_docked_window_label) = 0;
    virtual ImGuiID DockSpace(ImGuiID dockspace_id,
        const ImVec2& size = ImVec2(0, 0),
        ImGuiDockNodeFlags flags = 0,
        const ImGuiWindowClass* window_class = NULL) = 0;
    virtual ImGuiID DockSpaceOverViewport(ImGuiID dockspace_id = 0,
        const ImGuiViewport* viewport = NULL,
        ImGuiDockNodeFlags flags = 0,
        const ImGuiWindowClass* window_class = NULL) = 0;
    virtual void SetNextWindowDockID(ImGuiID dock_id, ImGuiCond cond = 0) = 0;
    virtual void SetNextWindowClass(const ImGuiWindowClass* window_class) = 0;
    virtual ImGuiID GetWindowDockID() = 0;
    virtual bool IsWindowDocked() = 0;
    virtual void LogToTTY(int auto_open_depth = -1) = 0;
    virtual void LogToFile(
        int auto_open_depth = -1, const char* filename = NULL) = 0;
    virtual void LogToClipboard(int auto_open_depth = -1) = 0;
    virtual void LogFinish() = 0;
    virtual void LogButtons() = 0;
    virtual void LogText(const char* fmt, ...) = 0;
    virtual void LogTextV(const char* fmt, va_list args) = 0;
    virtual bool BeginDragDropSource(ImGuiDragDropFlags flags = 0) = 0;
    virtual bool SetDragDropPayload(
        const char* type, const void* data, size_t sz, ImGuiCond cond = 0) = 0;
    virtual void EndDragDropSource() = 0;
    virtual bool BeginDragDropTarget() = 0;
    virtual const ImGuiPayload* AcceptDragDropPayload(
        const char* type, ImGuiDragDropFlags flags = 0) = 0;
    virtual void EndDragDropTarget() = 0;
    virtual const ImGuiPayload* GetDragDropPayload() = 0;
    virtual void BeginDisabled(bool disabled = true) = 0;
    virtual void EndDisabled() = 0;
    virtual void PushClipRect(const ImVec2& clip_rect_min,
        const ImVec2& clip_rect_max,
        bool intersect_with_current_clip_rect) = 0;
    virtual void PopClipRect() = 0;
    virtual void SetItemDefaultFocus() = 0;
    virtual void SetKeyboardFocusHere(int offset = 0) = 0;
    virtual void SetNavCursorVisible(bool visible) = 0;
    virtual void SetNextItemAllowOverlap() = 0;
    virtual bool IsItemHovered(ImGuiHoveredFlags flags = 0) = 0;
    virtual bool IsItemActive() = 0;
    virtual bool IsItemFocused() = 0;
    virtual bool IsItemClicked(ImGuiMouseButton mouse_button = 0) = 0;
    virtual bool IsItemVisible() = 0;
    virtual bool IsItemEdited() = 0;
    virtual bool IsItemActivated() = 0;
    virtual bool IsItemDeactivated() = 0;
    virtual bool IsItemDeactivatedAfterEdit() = 0;
    virtual bool IsItemToggledOpen() = 0;
    virtual bool IsAnyItemHovered() = 0;
    virtual bool IsAnyItemActive() = 0;
    virtual bool IsAnyItemFocused() = 0;
    virtual ImGuiID GetItemID() = 0;
    virtual ImVec2 GetItemRectMin() = 0;
    virtual ImVec2 GetItemRectMax() = 0;
    virtual ImVec2 GetItemRectSize() = 0;
    virtual ImGuiItemFlags GetItemFlags() = 0;
    virtual int GetItemClickedCountWithSingleClickDelay(
        ImGuiMouseButton mouse_button = 0, float delay = -1.0f) = 0;
    virtual ImGuiViewport* GetMainViewport() = 0;
    virtual ImDrawList* GetBackgroundDrawList(
        ImGuiViewport* viewport = NULL) = 0;
    virtual ImDrawList* GetForegroundDrawList(
        ImGuiViewport* viewport = NULL) = 0;
    virtual bool IsRectVisible(const ImVec2& size) = 0;
    virtual bool IsRectVisible(
        const ImVec2& rect_min, const ImVec2& rect_max) = 0;
    virtual double GetTime() = 0;
    virtual int GetFrameCount() = 0;
    virtual ImDrawListSharedData* GetDrawListSharedData() = 0;
    virtual const char* GetStyleColorName(ImGuiCol idx) = 0;
    virtual void SetStateStorage(ImGuiStorage* storage) = 0;
    virtual ImGuiStorage* GetStateStorage() = 0;
    virtual ImVec2 CalcTextSize(const char* text,
        const char* text_end = NULL,
        bool hide_text_after_double_hash = false,
        float wrap_width = -1.0f) = 0;
    virtual ImVec4 ColorConvertU32ToFloat4(ImU32 in) = 0;
    virtual ImU32 ColorConvertFloat4ToU32(const ImVec4& in) = 0;
    virtual void ColorConvertRGBtoHSV(float r,
        float g,
        float b,
        float& out_h,
        float& out_s,
        float& out_v) = 0;
    virtual void ColorConvertHSVtoRGB(float h,
        float s,
        float v,
        float& out_r,
        float& out_g,
        float& out_b) = 0;
    virtual bool IsKeyDown(ImGuiKey key) = 0;
    virtual bool IsKeyPressed(ImGuiKey key, bool repeat = true) = 0;
    virtual bool IsKeyReleased(ImGuiKey key) = 0;
    virtual bool IsKeyChordPressed(ImGuiKeyChord key_chord) = 0;
    virtual int GetKeyPressedAmount(
        ImGuiKey key, float repeat_delay, float rate) = 0;
    virtual const char* GetKeyName(ImGuiKey key) = 0;
    virtual void SetNextFrameWantCaptureKeyboard(
        bool want_capture_keyboard) = 0;
    virtual bool Shortcut(
        ImGuiKeyChord key_chord, ImGuiInputFlags flags = 0) = 0;
    virtual void SetNextItemShortcut(
        ImGuiKeyChord key_chord, ImGuiInputFlags flags = 0) = 0;
    virtual bool SetItemKeyOwner(ImGuiKey key) = 0;
    virtual bool IsMouseDown(ImGuiMouseButton button) = 0;
    virtual bool IsMouseClicked(
        ImGuiMouseButton button, bool repeat = false) = 0;
    virtual bool IsMouseReleased(ImGuiMouseButton button) = 0;
    virtual bool IsMouseDoubleClicked(ImGuiMouseButton button) = 0;
    virtual bool IsMouseReleasedWithDelay(
        ImGuiMouseButton button, float delay = -1.f) = 0;
    virtual int GetMouseClickedCount(ImGuiMouseButton button) = 0;
    virtual bool IsMouseHoveringRect(
        const ImVec2& r_min, const ImVec2& r_max, bool clip = true) = 0;
    virtual bool IsMousePosValid(const ImVec2* mouse_pos = NULL) = 0;
    virtual bool IsAnyMouseDown() = 0;
    virtual ImVec2 GetMousePos() = 0;
    virtual ImVec2 GetMousePosOnOpeningCurrentPopup() = 0;
    virtual bool IsMouseDragging(
        ImGuiMouseButton button, float lock_threshold = -1.0f) = 0;
    virtual ImVec2 GetMouseDragDelta(
        ImGuiMouseButton button = 0, float lock_threshold = -1.0f) = 0;
    virtual void ResetMouseDragDelta(ImGuiMouseButton button = 0) = 0;
    virtual ImGuiMouseCursor GetMouseCursor() = 0;
    virtual void SetMouseCursor(ImGuiMouseCursor cursor_type) = 0;
    virtual void SetNextFrameWantCaptureMouse(bool want_capture_mouse) = 0;
    virtual const char* GetClipboardText() = 0;
    virtual void SetClipboardText(const char* text) = 0;
    virtual void LoadIniSettingsFromDisk(const char* ini_filename) = 0;
    virtual void LoadIniSettingsFromMemory(
        const char* ini_data, size_t ini_size = 0) = 0;
    virtual void SaveIniSettingsToDisk(const char* ini_filename) = 0;
    virtual const char* SaveIniSettingsToMemory(
        size_t* out_ini_size = NULL) = 0;
    virtual void DebugTextEncoding(const char* text) = 0;
    virtual void DebugFlashStyleColor(ImGuiCol idx) = 0;
    virtual void DebugStartItemPicker() = 0;
    virtual bool DebugCheckVersionAndDataLayout(const char* version_str,
        size_t sz_io,
        size_t sz_style,
        size_t sz_vec2,
        size_t sz_vec4,
        size_t sz_drawvert,
        size_t sz_drawidx) = 0;
    virtual void DebugLog(const char* fmt, ...) = 0;
    virtual void DebugLogV(const char* fmt, va_list args) = 0;
    virtual void SetAllocatorFunctions(ImGuiMemAllocFunc alloc_func,
        ImGuiMemFreeFunc free_func,
        void* user_data = NULL) = 0;
    virtual void GetAllocatorFunctions(ImGuiMemAllocFunc* p_alloc_func,
        ImGuiMemFreeFunc* p_free_func,
        void** p_user_data) = 0;
    virtual void* MemAlloc(size_t size) = 0;
    virtual void MemFree(void* ptr) = 0;
    virtual void UpdatePlatformWindows() = 0;
    virtual void RenderPlatformWindowsDefault(
        void* platform_render_arg = NULL, void* renderer_render_arg = NULL) = 0;
    virtual void DestroyPlatformWindows() = 0;
    virtual ImGuiViewport* FindViewportByID(ImGuiID viewport_id) = 0;
    virtual ImGuiViewport* FindViewportByPlatformHandle(
        void* platform_handle) = 0;
    virtual ImPlotContext* ImPlot_CreateContext() = 0;
    virtual void ImPlot_DestroyContext(ImPlotContext* ctx = nullptr) = 0;
    virtual ImPlotContext* ImPlot_GetCurrentContext() = 0;
    virtual void ImPlot_SetCurrentContext(ImPlotContext* ctx) = 0;
    virtual void SetImGuiContext(ImGuiContext* ctx) = 0;
    virtual bool BeginPlot(const char* title_id,
        const ImVec2& size = ImVec2(-1, 0),
        ImPlotFlags flags = 0) = 0;
    virtual void EndPlot() = 0;
    virtual bool BeginSubplots(const char* title_id,
        int rows,
        int cols,
        const ImVec2& size,
        ImPlotSubplotFlags flags = 0,
        float* row_ratios = nullptr,
        float* col_ratios = nullptr) = 0;
    virtual void EndSubplots() = 0;
    virtual void SetupAxis(ImAxis axis,
        const char* label = nullptr,
        ImPlotAxisFlags flags = 0) = 0;
    virtual void SetupAxisLimits(ImAxis axis,
        double v_min,
        double v_max,
        ImPlotCond cond = ImPlotCond_Once) = 0;
    virtual void SetupAxisLinks(
        ImAxis axis, double* link_min, double* link_max) = 0;
    virtual void SetupAxisFormat(ImAxis axis, const char* fmt) = 0;
    virtual void SetupAxisFormat(
        ImAxis axis, ImPlotFormatter formatter, void* data = nullptr) = 0;
    virtual void SetupAxisTicks(ImAxis axis,
        const double* values,
        int n_ticks,
        const char* const labels[] = nullptr,
        bool keep_default = false) = 0;
    virtual void SetupAxisTicks(ImAxis axis,
        double v_min,
        double v_max,
        int n_ticks,
        const char* const labels[] = nullptr,
        bool keep_default = false) = 0;
    virtual void SetupAxisScale(ImAxis axis, ImPlotScale scale) = 0;
    virtual void SetupAxisScale(ImAxis axis,
        ImPlotTransform forward,
        ImPlotTransform inverse,
        void* data = nullptr) = 0;
    virtual void SetupAxisLimitsConstraints(
        ImAxis axis, double v_min, double v_max) = 0;
    virtual void SetupAxisZoomConstraints(
        ImAxis axis, double z_min, double z_max) = 0;
    virtual void SetupAxes(const char* x_label,
        const char* y_label,
        ImPlotAxisFlags x_flags = 0,
        ImPlotAxisFlags y_flags = 0) = 0;
    virtual void SetupAxesLimits(double x_min,
        double x_max,
        double y_min,
        double y_max,
        ImPlotCond cond = ImPlotCond_Once) = 0;
    virtual void SetupLegend(
        ImPlotLocation location, ImPlotLegendFlags flags = 0) = 0;
    virtual void SetupMouseText(
        ImPlotLocation location, ImPlotMouseTextFlags flags = 0) = 0;
    virtual void SetupFinish() = 0;
    virtual void SetNextAxisLimits(ImAxis axis,
        double v_min,
        double v_max,
        ImPlotCond cond = ImPlotCond_Once) = 0;
    virtual void SetNextAxisLinks(
        ImAxis axis, double* link_min, double* link_max) = 0;
    virtual void SetNextAxisToFit(ImAxis axis) = 0;
    virtual void SetNextAxesLimits(double x_min,
        double x_max,
        double y_min,
        double y_max,
        ImPlotCond cond = ImPlotCond_Once) = 0;
    virtual void SetNextAxesToFit() = 0;
    virtual void PlotLineG(const char* label_id,
        ImPlotGetter getter,
        void* data,
        int count,
        const ImPlotSpec& spec = ImPlotSpec()) = 0;
    virtual void PlotScatterG(const char* label_id,
        ImPlotGetter getter,
        void* data,
        int count,
        const ImPlotSpec& spec = ImPlotSpec()) = 0;
    virtual void PlotStairsG(const char* label_id,
        ImPlotGetter getter,
        void* data,
        int count,
        const ImPlotSpec& spec = ImPlotSpec()) = 0;
    virtual void PlotShadedG(const char* label_id,
        ImPlotGetter getter1,
        void* data1,
        ImPlotGetter getter2,
        void* data2,
        int count,
        const ImPlotSpec& spec = ImPlotSpec()) = 0;
    virtual void PlotBarsG(const char* label_id,
        ImPlotGetter getter,
        void* data,
        int count,
        double bar_size,
        const ImPlotSpec& spec = ImPlotSpec()) = 0;
    virtual void PlotDigitalG(const char* label_id,
        ImPlotGetter getter,
        void* data,
        int count,
        const ImPlotSpec& spec = ImPlotSpec()) = 0;
    virtual void PlotImage(const char* label_id,
        ImTextureRef tex_ref,
        const ImPlotPoint& bounds_min,
        const ImPlotPoint& bounds_max,
        const ImVec2& uv0 = ImVec2(0, 0),
        const ImVec2& uv1 = ImVec2(1, 1),
        const ImVec4& tint_col = ImVec4(1, 1, 1, 1),
        const ImPlotSpec& spec = ImPlotSpec()) = 0;
    virtual void PlotImage(const char* label_id,
        ImTextureID tex_ref,
        const ImPlotPoint& bounds_min,
        const ImPlotPoint& bounds_max,
        const ImVec2& uv0 = ImVec2(0, 0),
        const ImVec2& uv1 = ImVec2(1, 1),
        const ImVec4& tint_col = ImVec4(1, 1, 1, 1),
        const ImPlotSpec& spec = ImPlotSpec()) = 0;
    virtual void PlotText(const char* text,
        double x,
        double y,
        const ImVec2& pix_offset = ImVec2(0, 0),
        const ImPlotSpec& spec = ImPlotSpec()) = 0;
    virtual void PlotDummy(
        const char* label_id, const ImPlotSpec& spec = ImPlotSpec()) = 0;
    virtual bool DragPoint(int id,
        double* x,
        double* y,
        const ImVec4& col,
        float size = 4,
        ImPlotDragToolFlags flags = 0,
        bool* out_clicked = nullptr,
        bool* out_hovered = nullptr,
        bool* out_held = nullptr) = 0;
    virtual bool DragLineX(int id,
        double* x,
        const ImVec4& col,
        float thickness = 1,
        ImPlotDragToolFlags flags = 0,
        bool* out_clicked = nullptr,
        bool* out_hovered = nullptr,
        bool* out_held = nullptr) = 0;
    virtual bool DragLineY(int id,
        double* y,
        const ImVec4& col,
        float thickness = 1,
        ImPlotDragToolFlags flags = 0,
        bool* out_clicked = nullptr,
        bool* out_hovered = nullptr,
        bool* out_held = nullptr) = 0;
    virtual bool DragRect(int id,
        double* x1,
        double* y1,
        double* x2,
        double* y2,
        const ImVec4& col,
        ImPlotDragToolFlags flags = 0,
        bool* out_clicked = nullptr,
        bool* out_hovered = nullptr,
        bool* out_held = nullptr) = 0;
    virtual void Annotation(double x,
        double y,
        const ImVec4& col,
        const ImVec2& pix_offset,
        bool clamp,
        bool round = false) = 0;
    virtual void Annotation(double x,
        double y,
        const ImVec4& col,
        const ImVec2& pix_offset,
        bool clamp,
        const char* fmt,
        ...) = 0;
    virtual void AnnotationV(double x,
        double y,
        const ImVec4& col,
        const ImVec2& pix_offset,
        bool clamp,
        const char* fmt,
        va_list args) = 0;
    virtual void TagX(double x, const ImVec4& col, bool round = false) = 0;
    virtual void TagX(double x, const ImVec4& col, const char* fmt, ...) = 0;
    virtual void TagXV(
        double x, const ImVec4& col, const char* fmt, va_list args) = 0;
    virtual void TagY(double y, const ImVec4& col, bool round = false) = 0;
    virtual void TagY(double y, const ImVec4& col, const char* fmt, ...) = 0;
    virtual void TagYV(
        double y, const ImVec4& col, const char* fmt, va_list args) = 0;
    virtual void SetAxis(ImAxis axis) = 0;
    virtual void SetAxes(ImAxis x_axis, ImAxis y_axis) = 0;
    virtual ImPlotPoint PixelsToPlot(const ImVec2& pix,
        ImAxis x_axis = IMPLOT_AUTO,
        ImAxis y_axis = IMPLOT_AUTO) = 0;
    virtual ImPlotPoint PixelsToPlot(float x,
        float y,
        ImAxis x_axis = IMPLOT_AUTO,
        ImAxis y_axis = IMPLOT_AUTO) = 0;
    virtual ImVec2 PlotToPixels(const ImPlotPoint& plt,
        ImAxis x_axis = IMPLOT_AUTO,
        ImAxis y_axis = IMPLOT_AUTO) = 0;
    virtual ImVec2 PlotToPixels(double x,
        double y,
        ImAxis x_axis = IMPLOT_AUTO,
        ImAxis y_axis = IMPLOT_AUTO) = 0;
    virtual ImVec2 GetPlotPos() = 0;
    virtual ImVec2 GetPlotSize() = 0;
    virtual ImPlotPoint GetPlotMousePos(
        ImAxis x_axis = IMPLOT_AUTO, ImAxis y_axis = IMPLOT_AUTO) = 0;
    virtual ImPlotRect GetPlotLimits(
        ImAxis x_axis = IMPLOT_AUTO, ImAxis y_axis = IMPLOT_AUTO) = 0;
    virtual bool IsPlotHovered() = 0;
    virtual bool IsAxisHovered(ImAxis axis) = 0;
    virtual bool IsSubplotsHovered() = 0;
    virtual bool IsPlotSelected() = 0;
    virtual ImPlotRect GetPlotSelection(
        ImAxis x_axis = IMPLOT_AUTO, ImAxis y_axis = IMPLOT_AUTO) = 0;
    virtual void CancelPlotSelection() = 0;
    virtual void HideNextItem(
        bool hidden = true, ImPlotCond cond = ImPlotCond_Once) = 0;
    virtual bool BeginAlignedPlots(
        const char* group_id, bool vertical = true) = 0;
    virtual void EndAlignedPlots() = 0;
    virtual bool BeginLegendPopup(
        const char* label_id, ImGuiMouseButton mouse_button = 1) = 0;
    virtual void EndLegendPopup() = 0;
    virtual bool IsLegendEntryHovered(const char* label_id) = 0;
    virtual bool BeginDragDropTargetPlot() = 0;
    virtual bool BeginDragDropTargetAxis(ImAxis axis) = 0;
    virtual bool BeginDragDropTargetLegend() = 0;
    virtual void ImPlot_EndDragDropTarget() = 0;
    virtual bool BeginDragDropSourcePlot(ImGuiDragDropFlags flags = 0) = 0;
    virtual bool BeginDragDropSourceAxis(
        ImAxis axis, ImGuiDragDropFlags flags = 0) = 0;
    virtual bool BeginDragDropSourceItem(
        const char* label_id, ImGuiDragDropFlags flags = 0) = 0;
    virtual void ImPlot_EndDragDropSource() = 0;
    virtual ImPlotStyle& ImPlot_GetStyle() = 0;
    virtual void StyleColorsAuto(ImPlotStyle* dst = nullptr) = 0;
    virtual void ImPlot_StyleColorsClassic(ImPlotStyle* dst = nullptr) = 0;
    virtual void ImPlot_StyleColorsDark(ImPlotStyle* dst = nullptr) = 0;
    virtual void ImPlot_StyleColorsLight(ImPlotStyle* dst = nullptr) = 0;
    virtual void ImPlot_PushStyleColor(ImPlotCol idx, ImU32 col) = 0;
    virtual void ImPlot_PushStyleColor(ImPlotCol idx, const ImVec4& col) = 0;
    virtual void ImPlot_PopStyleColor(int count = 1) = 0;
    virtual void ImPlot_PushStyleVar(ImPlotStyleVar idx, float val) = 0;
    virtual void ImPlot_PushStyleVar(ImPlotStyleVar idx, int val) = 0;
    virtual void ImPlot_PushStyleVar(ImPlotStyleVar idx, const ImVec2& val) = 0;
    virtual void ImPlot_PopStyleVar(int count = 1) = 0;
    virtual ImVec4 GetLastItemColor() = 0;
    virtual const char* ImPlot_GetStyleColorName(ImPlotCol idx) = 0;
    virtual const char* GetMarkerName(ImPlotMarker idx) = 0;
    virtual ImPlotMarker NextMarker() = 0;
    virtual ImPlotColormap AddColormap(
        const char* name, const ImVec4* cols, int size, bool qual = true) = 0;
    virtual ImPlotColormap AddColormap(
        const char* name, const ImU32* cols, int size, bool qual = true) = 0;
    virtual int GetColormapCount() = 0;
    virtual const char* GetColormapName(ImPlotColormap cmap) = 0;
    virtual ImPlotColormap GetColormapIndex(const char* name) = 0;
    virtual void PushColormap(ImPlotColormap cmap) = 0;
    virtual void PushColormap(const char* name) = 0;
    virtual void PopColormap(int count = 1) = 0;
    virtual ImVec4 NextColormapColor() = 0;
    virtual int GetColormapSize(ImPlotColormap cmap = IMPLOT_AUTO) = 0;
    virtual ImVec4 GetColormapColor(
        int idx, ImPlotColormap cmap = IMPLOT_AUTO) = 0;
    virtual ImVec4 SampleColormap(
        float t, ImPlotColormap cmap = IMPLOT_AUTO) = 0;
    virtual void ColormapScale(const char* label,
        double scale_min,
        double scale_max,
        const ImVec2& size = ImVec2(0, 0),
        const char* format = "%g",
        ImPlotColormapScaleFlags flags = 0,
        ImPlotColormap cmap = IMPLOT_AUTO) = 0;
    virtual bool ColormapSlider(const char* label,
        float* t,
        ImVec4* out = nullptr,
        const char* format = "",
        ImPlotColormap cmap = IMPLOT_AUTO) = 0;
    virtual bool ColormapButton(const char* label,
        const ImVec2& size = ImVec2(0, 0),
        ImPlotColormap cmap = IMPLOT_AUTO) = 0;
    virtual void BustColorCache(const char* plot_title_id = nullptr) = 0;
    virtual ImPlotInputMap& GetInputMap() = 0;
    virtual void MapInputDefault(ImPlotInputMap* dst = nullptr) = 0;
    virtual void MapInputReverse(ImPlotInputMap* dst = nullptr) = 0;
    virtual void ItemIcon(const ImVec4& col) = 0;
    virtual void ItemIcon(ImU32 col) = 0;
    virtual void ColormapIcon(ImPlotColormap cmap) = 0;
    virtual ImDrawList* GetPlotDrawList() = 0;
    virtual void PushPlotClipRect(float expand = 0) = 0;
    virtual void PopPlotClipRect() = 0;
    virtual bool ImPlot_ShowStyleSelector(const char* label) = 0;
    virtual bool ShowColormapSelector(const char* label) = 0;
    virtual bool ShowInputMapSelector(const char* label) = 0;
    virtual void ImPlot_ShowStyleEditor(ImPlotStyle* ref = nullptr) = 0;
    virtual void ImPlot_ShowUserGuide() = 0;
    virtual void ImPlot_ShowMetricsWindow(bool* p_popen = nullptr) = 0;
    virtual void ImPlot_ShowDemoWindow(bool* p_open = nullptr) = 0;
    virtual void SetColorEditOptions(ImGuiColorEditFlags flags) = 0;
    virtual void SetWindowFontScale(float scale) = 0;
    virtual void Image(ImTextureRef tex_ref,
        const ImVec2& image_size,
        const ImVec2& uv0,
        const ImVec2& uv1,
        const ImVec4& tint_col,
        const ImVec4& border_col) = 0;
    virtual ImVec2 GetContentRegionMax() = 0;
    virtual ImVec2 GetWindowContentRegionMin() = 0;
    virtual ImVec2 GetWindowContentRegionMax() = 0;

    // Templated ImPlot entry points
    template <typename T>
    void PlotLine(const char* label_id,
        const T* values,
        int count,
        double xscale = 1,
        double xstart = 0,
        const ImPlotSpec& spec = ImPlotSpec())
    {
        ImPlot::PlotLine(label_id, values, count, xscale, xstart, spec);
    }
    template <typename T>
    void PlotLine(const char* label_id,
        const T* xs,
        const T* ys,
        int count,
        const ImPlotSpec& spec = ImPlotSpec())
    {
        ImPlot::PlotLine(label_id, xs, ys, count, spec);
    }
    template <typename T>
    void PlotScatter(const char* label_id,
        const T* values,
        int count,
        double xscale = 1,
        double xstart = 0,
        const ImPlotSpec& spec = ImPlotSpec())
    {
        ImPlot::PlotScatter(label_id, values, count, xscale, xstart, spec);
    }
    template <typename T>
    void PlotScatter(const char* label_id,
        const T* xs,
        const T* ys,
        int count,
        const ImPlotSpec& spec = ImPlotSpec())
    {
        ImPlot::PlotScatter(label_id, xs, ys, count, spec);
    }
    template <typename T>
    void PlotBubbles(const char* label_id,
        const T* values,
        const T* szs,
        int count,
        double xscale = 1,
        double xstart = 0,
        const ImPlotSpec& spec = ImPlotSpec())
    {
        ImPlot::PlotBubbles(label_id, values, szs, count, xscale, xstart, spec);
    }
    template <typename T>
    void PlotBubbles(const char* label_id,
        const T* xs,
        const T* ys,
        const T* szs,
        int count,
        const ImPlotSpec& spec = ImPlotSpec())
    {
        ImPlot::PlotBubbles(label_id, xs, ys, szs, count, spec);
    }
    template <typename T>
    void PlotPolygon(const char* label_id,
        const T* xs,
        const T* ys,
        int count,
        const ImPlotSpec& spec = ImPlotSpec())
    {
        ImPlot::PlotPolygon(label_id, xs, ys, count, spec);
    }
    template <typename T>
    void PlotStairs(const char* label_id,
        const T* values,
        int count,
        double xscale = 1,
        double xstart = 0,
        const ImPlotSpec& spec = ImPlotSpec())
    {
        ImPlot::PlotStairs(label_id, values, count, xscale, xstart, spec);
    }
    template <typename T>
    void PlotStairs(const char* label_id,
        const T* xs,
        const T* ys,
        int count,
        const ImPlotSpec& spec = ImPlotSpec())
    {
        ImPlot::PlotStairs(label_id, xs, ys, count, spec);
    }
    template <typename T>
    void PlotShaded(const char* label_id,
        const T* values,
        int count,
        double yref = 0,
        double xscale = 1,
        double xstart = 0,
        const ImPlotSpec& spec = ImPlotSpec())
    {
        ImPlot::PlotShaded(label_id, values, count, yref, xscale, xstart, spec);
    }
    template <typename T>
    void PlotShaded(const char* label_id,
        const T* xs,
        const T* ys,
        int count,
        double yref = 0,
        const ImPlotSpec& spec = ImPlotSpec())
    {
        ImPlot::PlotShaded(label_id, xs, ys, count, yref, spec);
    }
    template <typename T>
    void PlotShaded(const char* label_id,
        const T* xs,
        const T* ys1,
        const T* ys2,
        int count,
        const ImPlotSpec& spec = ImPlotSpec())
    {
        ImPlot::PlotShaded(label_id, xs, ys1, ys2, count, spec);
    }
    template <typename T>
    void PlotBars(const char* label_id,
        const T* values,
        int count,
        double bar_size = 0.67,
        double shift = 0,
        const ImPlotSpec& spec = ImPlotSpec())
    {
        ImPlot::PlotBars(label_id, values, count, bar_size, shift, spec);
    }
    template <typename T>
    void PlotBars(const char* label_id,
        const T* xs,
        const T* ys,
        int count,
        double bar_size,
        const ImPlotSpec& spec = ImPlotSpec())
    {
        ImPlot::PlotBars(label_id, xs, ys, count, bar_size, spec);
    }
    template <typename T>
    void PlotBarGroups(const char* const label_ids[],
        const T* values,
        int item_count,
        int group_count,
        double group_size = 0.67,
        double shift = 0,
        const ImPlotSpec& spec = ImPlotSpec())
    {
        ImPlot::PlotBarGroups(label_ids,
            values,
            item_count,
            group_count,
            group_size,
            shift,
            spec);
    }
    template <typename T>
    void PlotErrorBars(const char* label_id,
        const T* xs,
        const T* ys,
        const T* err,
        int count,
        const ImPlotSpec& spec = ImPlotSpec())
    {
        ImPlot::PlotErrorBars(label_id, xs, ys, err, count, spec);
    }
    template <typename T>
    void PlotErrorBars(const char* label_id,
        const T* xs,
        const T* ys,
        const T* neg,
        const T* pos,
        int count,
        const ImPlotSpec& spec = ImPlotSpec())
    {
        ImPlot::PlotErrorBars(label_id, xs, ys, neg, pos, count, spec);
    }
    template <typename T>
    void PlotStems(const char* label_id,
        const T* values,
        int count,
        double ref = 0,
        double scale = 1,
        double start = 0,
        const ImPlotSpec& spec = ImPlotSpec())
    {
        ImPlot::PlotStems(label_id, values, count, ref, scale, start, spec);
    }
    template <typename T>
    void PlotStems(const char* label_id,
        const T* xs,
        const T* ys,
        int count,
        double ref = 0,
        const ImPlotSpec& spec = ImPlotSpec())
    {
        ImPlot::PlotStems(label_id, xs, ys, count, ref, spec);
    }
    template <typename T>
    void PlotInfLines(const char* label_id,
        const T* values,
        int count,
        const ImPlotSpec& spec = ImPlotSpec())
    {
        ImPlot::PlotInfLines(label_id, values, count, spec);
    }
    template <typename T>
    void PlotPieChart(const char* const label_ids[],
        const T* values,
        int count,
        double x,
        double y,
        double radius,
        ImPlotFormatter fmt,
        void* fmt_data = nullptr,
        double angle0 = 90,
        const ImPlotSpec& spec = ImPlotSpec())
    {
        ImPlot::PlotPieChart(label_ids,
            values,
            count,
            x,
            y,
            radius,
            fmt,
            fmt_data,
            angle0,
            spec);
    }
    template <typename T>
    void PlotPieChart(const char* const label_ids[],
        const T* values,
        int count,
        double x,
        double y,
        double radius,
        const char* label_fmt = "%.1f",
        double angle0 = 90,
        const ImPlotSpec& spec = ImPlotSpec())
    {
        ImPlot::PlotPieChart(
            label_ids, values, count, x, y, radius, label_fmt, angle0, spec);
    }
    template <typename T>
    void PlotHeatmap(const char* label_id,
        const T* values,
        int rows,
        int cols,
        double scale_min = 0,
        double scale_max = 0,
        const char* label_fmt = "%.1f",
        const ImPlotPoint& bounds_min = ImPlotPoint(0, 0),
        const ImPlotPoint& bounds_max = ImPlotPoint(1, 1),
        const ImPlotSpec& spec = ImPlotSpec())
    {
        ImPlot::PlotHeatmap(label_id,
            values,
            rows,
            cols,
            scale_min,
            scale_max,
            label_fmt,
            bounds_min,
            bounds_max,
            spec);
    }
    template <typename T>
    double PlotHistogram(const char* label_id,
        const T* values,
        int count,
        int bins = ImPlotBin_Sturges,
        double bar_scale = 1.0,
        ImPlotRange range = ImPlotRange(),
        const ImPlotSpec& spec = ImPlotSpec())
    {
        return ImPlot::PlotHistogram(
            label_id, values, count, bins, bar_scale, range, spec);
    }
    template <typename T>
    double PlotHistogram2D(const char* label_id,
        const T* xs,
        const T* ys,
        int count,
        int x_bins = ImPlotBin_Sturges,
        int y_bins = ImPlotBin_Sturges,
        ImPlotRect range = ImPlotRect(),
        const ImPlotSpec& spec = ImPlotSpec())
    {
        return ImPlot::PlotHistogram2D(
            label_id, xs, ys, count, x_bins, y_bins, range, spec);
    }
    template <typename T>
    void PlotDigital(const char* label_id,
        const T* xs,
        const T* ys,
        int count,
        const ImPlotSpec& spec = ImPlotSpec())
    {
        ImPlot::PlotDigital(label_id, xs, ys, count, spec);
    }
};

extern std::unique_ptr<ImGuiIf> CreateImGui();
