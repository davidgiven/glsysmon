#include "imguiif.h"

#include <cstdarg>
#include <imgui.h>
#include <implot.h>

namespace
{

    class ImGuiIfImpl : public ImGuiIf
    {
    public:
        ImGuiContext* CreateContext(
            ImFontAtlas* shared_font_atlas = NULL) override
        {
            return ImGui::CreateContext(shared_font_atlas);
        }
        void DestroyContext(ImGuiContext* ctx = NULL) override
        {
            ImGui::DestroyContext(ctx);
        }
        ImGuiContext* GetCurrentContext() override
        {
            return ImGui::GetCurrentContext();
        }
        void SetCurrentContext(ImGuiContext* ctx) override
        {
            ImGui::SetCurrentContext(ctx);
        }
        ImGuiIO& GetIO() override
        {
            return ImGui::GetIO();
        }
        ImGuiPlatformIO& GetPlatformIO() override
        {
            return ImGui::GetPlatformIO();
        }
        ImGuiStyle& GetStyle() override
        {
            return ImGui::GetStyle();
        }
        void NewFrame() override
        {
            ImGui::NewFrame();
        }
        void EndFrame() override
        {
            ImGui::EndFrame();
        }
        void Render() override
        {
            ImGui::Render();
        }
        ImDrawData* GetDrawData() override
        {
            return ImGui::GetDrawData();
        }
        void ShowDemoWindow(bool* p_open = NULL) override
        {
            (void)p_open;
        }
        void ShowMetricsWindow(bool* p_open = NULL) override
        {
            (void)p_open;
        }
        void ShowDebugLogWindow(bool* p_open = NULL) override
        {
            (void)p_open;
        }
        void ShowIDStackToolWindow(bool* p_open = NULL) override
        {
            (void)p_open;
        }
        void ShowAboutWindow(bool* p_open = NULL) override
        {
            (void)p_open;
        }
        void ShowStyleEditor(ImGuiStyle* ref = NULL) override
        {
            (void)ref;
        }
        bool ShowStyleSelector(const char* label) override
        {
            (void)label;
            return false;
        }
        void ShowFontSelector(const char* label) override
        {
            (void)label;
        }
        void ShowUserGuide() override {}
        const char* GetVersion() override
        {
            return ImGui::GetVersion();
        }
        void StyleColorsDark(ImGuiStyle* dst = NULL) override
        {
            ImGui::StyleColorsDark(dst);
        }
        void StyleColorsLight(ImGuiStyle* dst = NULL) override
        {
            ImGui::StyleColorsLight(dst);
        }
        void StyleColorsClassic(ImGuiStyle* dst = NULL) override
        {
            ImGui::StyleColorsClassic(dst);
        }
        bool Begin(const char* name,
            bool* p_open = NULL,
            ImGuiWindowFlags flags = 0) override
        {
            return ImGui::Begin(name, p_open, flags);
        }
        void End() override
        {
            ImGui::End();
        }
        bool BeginChild(const char* str_id,
            const ImVec2& size = ImVec2(0, 0),
            ImGuiChildFlags child_flags = 0,
            ImGuiWindowFlags window_flags = 0) override
        {
            return ImGui::BeginChild(str_id, size, child_flags, window_flags);
        }
        bool BeginChild(ImGuiID id,
            const ImVec2& size = ImVec2(0, 0),
            ImGuiChildFlags child_flags = 0,
            ImGuiWindowFlags window_flags = 0) override
        {
            return ImGui::BeginChild(id, size, child_flags, window_flags);
        }
        void EndChild() override
        {
            ImGui::EndChild();
        }
        bool IsWindowAppearing() override
        {
            return ImGui::IsWindowAppearing();
        }
        bool IsWindowCollapsed() override
        {
            return ImGui::IsWindowCollapsed();
        }
        bool IsWindowFocused(ImGuiFocusedFlags flags = 0) override
        {
            return ImGui::IsWindowFocused(flags);
        }
        bool IsWindowHovered(ImGuiHoveredFlags flags = 0) override
        {
            return ImGui::IsWindowHovered(flags);
        }
        ImDrawList* GetWindowDrawList() override
        {
            return ImGui::GetWindowDrawList();
        }
        float GetWindowDpiScale() override
        {
            return ImGui::GetWindowDpiScale();
        }
        ImVec2 GetWindowPos() override
        {
            return ImGui::GetWindowPos();
        }
        ImVec2 GetWindowSize() override
        {
            return ImGui::GetWindowSize();
        }
        float GetWindowWidth() override
        {
            return ImGui::GetWindowWidth();
        }
        float GetWindowHeight() override
        {
            return ImGui::GetWindowHeight();
        }
        ImGuiViewport* GetWindowViewport() override
        {
            return ImGui::GetWindowViewport();
        }
        void SetNextWindowPos(const ImVec2& pos,
            ImGuiCond cond = 0,
            const ImVec2& pivot = ImVec2(0, 0)) override
        {
            ImGui::SetNextWindowPos(pos, cond, pivot);
        }
        void SetNextWindowSize(const ImVec2& size, ImGuiCond cond = 0) override
        {
            ImGui::SetNextWindowSize(size, cond);
        }
        void SetNextWindowSizeConstraints(const ImVec2& size_min,
            const ImVec2& size_max,
            ImGuiSizeCallback custom_callback = NULL,
            void* custom_callback_data = NULL) override
        {
            ImGui::SetNextWindowSizeConstraints(
                size_min, size_max, custom_callback, custom_callback_data);
        }
        void SetNextWindowContentSize(const ImVec2& size) override
        {
            ImGui::SetNextWindowContentSize(size);
        }
        void SetNextWindowCollapsed(bool collapsed, ImGuiCond cond = 0) override
        {
            ImGui::SetNextWindowCollapsed(collapsed, cond);
        }
        void SetNextWindowFocus() override
        {
            ImGui::SetNextWindowFocus();
        }
        void SetNextWindowScroll(const ImVec2& scroll) override
        {
            ImGui::SetNextWindowScroll(scroll);
        }
        void SetNextWindowBgAlpha(float alpha) override
        {
            ImGui::SetNextWindowBgAlpha(alpha);
        }
        void SetNextWindowViewport(ImGuiID viewport_id) override
        {
            ImGui::SetNextWindowViewport(viewport_id);
        }
        void SetWindowPos(const ImVec2& pos, ImGuiCond cond = 0) override
        {
            ImGui::SetWindowPos(pos, cond);
        }
        void SetWindowSize(const ImVec2& size, ImGuiCond cond = 0) override
        {
            ImGui::SetWindowSize(size, cond);
        }
        void SetWindowCollapsed(bool collapsed, ImGuiCond cond = 0) override
        {
            ImGui::SetWindowCollapsed(collapsed, cond);
        }
        void SetWindowFocus() override
        {
            ImGui::SetWindowFocus();
        }
        void SetWindowPos(
            const char* name, const ImVec2& pos, ImGuiCond cond = 0) override
        {
            ImGui::SetWindowPos(name, pos, cond);
        }
        void SetWindowSize(
            const char* name, const ImVec2& size, ImGuiCond cond = 0) override
        {
            ImGui::SetWindowSize(name, size, cond);
        }
        void SetWindowCollapsed(
            const char* name, bool collapsed, ImGuiCond cond = 0) override
        {
            ImGui::SetWindowCollapsed(name, collapsed, cond);
        }
        void SetWindowFocus(const char* name) override
        {
            ImGui::SetWindowFocus(name);
        }
        float GetScrollX() override
        {
            return ImGui::GetScrollX();
        }
        float GetScrollY() override
        {
            return ImGui::GetScrollY();
        }
        void SetScrollX(float scroll_x) override
        {
            ImGui::SetScrollX(scroll_x);
        }
        void SetScrollY(float scroll_y) override
        {
            ImGui::SetScrollY(scroll_y);
        }
        float GetScrollMaxX() override
        {
            return ImGui::GetScrollMaxX();
        }
        float GetScrollMaxY() override
        {
            return ImGui::GetScrollMaxY();
        }
        void SetScrollHereX(float center_x_ratio = 0.5f) override
        {
            ImGui::SetScrollHereX(center_x_ratio);
        }
        void SetScrollHereY(float center_y_ratio = 0.5f) override
        {
            ImGui::SetScrollHereY(center_y_ratio);
        }
        void SetScrollFromPosX(
            float local_x, float center_x_ratio = 0.5f) override
        {
            ImGui::SetScrollFromPosX(local_x, center_x_ratio);
        }
        void SetScrollFromPosY(
            float local_y, float center_y_ratio = 0.5f) override
        {
            ImGui::SetScrollFromPosY(local_y, center_y_ratio);
        }
        void PushFont(ImFont* font, float font_size_base_unscaled) override
        {
            ImGui::PushFont(font, font_size_base_unscaled);
        }
        void PopFont() override
        {
            ImGui::PopFont();
        }
        ImFont* GetFont() override
        {
            return ImGui::GetFont();
        }
        float GetFontSize() override
        {
            return ImGui::GetFontSize();
        }
        ImFontBaked* GetFontBaked() override
        {
            return ImGui::GetFontBaked();
        }
        void PushStyleColor(ImGuiCol idx, ImU32 col) override
        {
            ImGui::PushStyleColor(idx, col);
        }
        void PushStyleColor(ImGuiCol idx, const ImVec4& col) override
        {
            ImGui::PushStyleColor(idx, col);
        }
        void PopStyleColor(int count = 1) override
        {
            ImGui::PopStyleColor(count);
        }
        void PushStyleVar(ImGuiStyleVar idx, float val) override
        {
            ImGui::PushStyleVar(idx, val);
        }
        void PushStyleVar(ImGuiStyleVar idx, const ImVec2& val) override
        {
            ImGui::PushStyleVar(idx, val);
        }
        void PushStyleVarX(ImGuiStyleVar idx, float val_x) override
        {
            ImGui::PushStyleVarX(idx, val_x);
        }
        void PushStyleVarY(ImGuiStyleVar idx, float val_y) override
        {
            ImGui::PushStyleVarY(idx, val_y);
        }
        void PopStyleVar(int count = 1) override
        {
            ImGui::PopStyleVar(count);
        }
        void PushItemFlag(ImGuiItemFlags option, bool enabled) override
        {
            ImGui::PushItemFlag(option, enabled);
        }
        void PopItemFlag() override
        {
            ImGui::PopItemFlag();
        }
        void PushItemWidth(float item_width) override
        {
            ImGui::PushItemWidth(item_width);
        }
        void PopItemWidth() override
        {
            ImGui::PopItemWidth();
        }
        void SetNextItemWidth(float item_width) override
        {
            ImGui::SetNextItemWidth(item_width);
        }
        float CalcItemWidth() override
        {
            return ImGui::CalcItemWidth();
        }
        void PushTextWrapPos(float wrap_local_pos_x = 0.0f) override
        {
            ImGui::PushTextWrapPos(wrap_local_pos_x);
        }
        void PopTextWrapPos() override
        {
            ImGui::PopTextWrapPos();
        }
        ImVec2 GetFontTexUvWhitePixel() override
        {
            return ImGui::GetFontTexUvWhitePixel();
        }
        ImU32 GetColorU32(ImGuiCol idx, float alpha_mul = 1.0f) override
        {
            return ImGui::GetColorU32(idx, alpha_mul);
        }
        ImU32 GetColorU32(const ImVec4& col) override
        {
            return ImGui::GetColorU32(col);
        }
        ImU32 GetColorU32(ImU32 col, float alpha_mul = 1.0f) override
        {
            return ImGui::GetColorU32(col, alpha_mul);
        }
        const ImVec4& GetStyleColorVec4(ImGuiCol idx) override
        {
            return ImGui::GetStyleColorVec4(idx);
        }
        ImVec2 GetCursorScreenPos() override
        {
            return ImGui::GetCursorScreenPos();
        }
        void SetCursorScreenPos(const ImVec2& pos) override
        {
            ImGui::SetCursorScreenPos(pos);
        }
        ImVec2 GetContentRegionAvail() override
        {
            return ImGui::GetContentRegionAvail();
        }
        ImVec2 GetCursorPos() override
        {
            return ImGui::GetCursorPos();
        }
        float GetCursorPosX() override
        {
            return ImGui::GetCursorPosX();
        }
        float GetCursorPosY() override
        {
            return ImGui::GetCursorPosY();
        }
        void SetCursorPos(const ImVec2& local_pos) override
        {
            ImGui::SetCursorPos(local_pos);
        }
        void SetCursorPosX(float local_x) override
        {
            ImGui::SetCursorPosX(local_x);
        }
        void SetCursorPosY(float local_y) override
        {
            ImGui::SetCursorPosY(local_y);
        }
        ImVec2 GetCursorStartPos() override
        {
            return ImGui::GetCursorStartPos();
        }
        void Separator() override
        {
            ImGui::Separator();
        }
        void SameLine(
            float offset_from_start_x = 0.0f, float spacing = -1.0f) override
        {
            ImGui::SameLine(offset_from_start_x, spacing);
        }
        void NewLine() override
        {
            ImGui::NewLine();
        }
        void Spacing() override
        {
            ImGui::Spacing();
        }
        void Dummy(const ImVec2& size) override
        {
            ImGui::Dummy(size);
        }
        void Indent(float indent_w = 0.0f) override
        {
            ImGui::Indent(indent_w);
        }
        void Unindent(float indent_w = 0.0f) override
        {
            ImGui::Unindent(indent_w);
        }
        void BeginGroup() override
        {
            ImGui::BeginGroup();
        }
        void EndGroup() override
        {
            ImGui::EndGroup();
        }
        void AlignTextToFramePadding() override
        {
            ImGui::AlignTextToFramePadding();
        }
        float GetTextLineHeight() override
        {
            return ImGui::GetTextLineHeight();
        }
        float GetTextLineHeightWithSpacing() override
        {
            return ImGui::GetTextLineHeightWithSpacing();
        }
        float GetFrameHeight() override
        {
            return ImGui::GetFrameHeight();
        }
        float GetFrameHeightWithSpacing() override
        {
            return ImGui::GetFrameHeightWithSpacing();
        }
        void PushID(const char* str_id) override
        {
            ImGui::PushID(str_id);
        }
        void PushID(const char* str_id_begin, const char* str_id_end) override
        {
            ImGui::PushID(str_id_begin, str_id_end);
        }
        void PushID(const void* ptr_id) override
        {
            ImGui::PushID(ptr_id);
        }
        void PushID(int int_id) override
        {
            ImGui::PushID(int_id);
        }
        void PopID() override
        {
            ImGui::PopID();
        }
        ImGuiID GetID(const char* str_id) override
        {
            return ImGui::GetID(str_id);
        }
        ImGuiID GetID(const char* str_id_begin, const char* str_id_end) override
        {
            return ImGui::GetID(str_id_begin, str_id_end);
        }
        ImGuiID GetID(const void* ptr_id) override
        {
            return ImGui::GetID(ptr_id);
        }
        ImGuiID GetID(int int_id) override
        {
            return ImGui::GetID(int_id);
        }
        void TextUnformatted(
            const char* text, const char* text_end = NULL) override
        {
            ImGui::TextUnformatted(text, text_end);
        }
        void Text(const char* fmt, ...) override
        {
            va_list args;
            va_start(args, fmt);
            ImGui::TextV(fmt, args);
            va_end(args);
        }
        void TextV(const char* fmt, va_list args) override
        {
            ImGui::TextV(fmt, args);
        }
        void TextColored(const ImVec4& col, const char* fmt, ...) override
        {
            va_list args;
            va_start(args, fmt);
            ImGui::TextColoredV(col, fmt, args);
            va_end(args);
        }
        void TextColoredV(
            const ImVec4& col, const char* fmt, va_list args) override
        {
            ImGui::TextColoredV(col, fmt, args);
        }
        void TextDisabled(const char* fmt, ...) override
        {
            va_list args;
            va_start(args, fmt);
            ImGui::TextDisabledV(fmt, args);
            va_end(args);
        }
        void TextDisabledV(const char* fmt, va_list args) override
        {
            ImGui::TextDisabledV(fmt, args);
        }
        void TextWrapped(const char* fmt, ...) override
        {
            va_list args;
            va_start(args, fmt);
            ImGui::TextWrappedV(fmt, args);
            va_end(args);
        }
        void TextWrappedV(const char* fmt, va_list args) override
        {
            ImGui::TextWrappedV(fmt, args);
        }
        void LabelText(const char* label, const char* fmt, ...) override
        {
            va_list args;
            va_start(args, fmt);
            ImGui::LabelTextV(label, fmt, args);
            va_end(args);
        }
        void LabelTextV(
            const char* label, const char* fmt, va_list args) override
        {
            ImGui::LabelTextV(label, fmt, args);
        }
        void BulletText(const char* fmt, ...) override
        {
            va_list args;
            va_start(args, fmt);
            ImGui::BulletTextV(fmt, args);
            va_end(args);
        }
        void BulletTextV(const char* fmt, va_list args) override
        {
            ImGui::BulletTextV(fmt, args);
        }
        void SeparatorText(const char* label) override
        {
            ImGui::SeparatorText(label);
        }
        bool Button(
            const char* label, const ImVec2& size = ImVec2(0, 0)) override
        {
            return ImGui::Button(label, size);
        }
        bool SmallButton(const char* label) override
        {
            return ImGui::SmallButton(label);
        }
        bool InvisibleButton(const char* str_id,
            const ImVec2& size,
            ImGuiButtonFlags flags = 0) override
        {
            return ImGui::InvisibleButton(str_id, size, flags);
        }
        bool ArrowButton(const char* str_id, ImGuiDir dir) override
        {
            return ImGui::ArrowButton(str_id, dir);
        }
        bool Checkbox(const char* label, bool* v) override
        {
            return ImGui::Checkbox(label, v);
        }
        bool CheckboxFlags(
            const char* label, int* flags, int flags_value) override
        {
            return ImGui::CheckboxFlags(label, flags, flags_value);
        }
        bool CheckboxFlags(const char* label,
            unsigned int* flags,
            unsigned int flags_value) override
        {
            return ImGui::CheckboxFlags(label, flags, flags_value);
        }
        bool RadioButton(const char* label, bool active) override
        {
            return ImGui::RadioButton(label, active);
        }
        bool RadioButton(const char* label, int* v, int v_button) override
        {
            return ImGui::RadioButton(label, v, v_button);
        }
        void ProgressBar(float fraction,
            const ImVec2& size_arg = ImVec2(-FLT_MIN, 0),
            const char* overlay = NULL) override
        {
            ImGui::ProgressBar(fraction, size_arg, overlay);
        }
        void Bullet() override
        {
            ImGui::Bullet();
        }
        bool TextLink(const char* label) override
        {
            return ImGui::TextLink(label);
        }
        bool TextLinkOpenURL(const char* label, const char* url = NULL) override
        {
            return ImGui::TextLinkOpenURL(label, url);
        }
        void Image(ImTextureRef tex_ref,
            const ImVec2& image_size,
            const ImVec2& uv0 = ImVec2(0, 0),
            const ImVec2& uv1 = ImVec2(1, 1)) override
        {
            ImGui::Image(tex_ref, image_size, uv0, uv1);
        }
        void ImageWithBg(ImTextureRef tex_ref,
            const ImVec2& image_size,
            const ImVec2& uv0 = ImVec2(0, 0),
            const ImVec2& uv1 = ImVec2(1, 1),
            const ImVec4& bg_col = ImVec4(0, 0, 0, 0),
            const ImVec4& tint_col = ImVec4(1, 1, 1, 1)) override
        {
            ImGui::ImageWithBg(tex_ref, image_size, uv0, uv1, bg_col, tint_col);
        }
        bool ImageButton(const char* str_id,
            ImTextureRef tex_ref,
            const ImVec2& image_size,
            const ImVec2& uv0 = ImVec2(0, 0),
            const ImVec2& uv1 = ImVec2(1, 1),
            const ImVec4& bg_col = ImVec4(0, 0, 0, 0),
            const ImVec4& tint_col = ImVec4(1, 1, 1, 1)) override
        {
            return ImGui::ImageButton(
                str_id, tex_ref, image_size, uv0, uv1, bg_col, tint_col);
        }
        bool BeginCombo(const char* label,
            const char* preview_value,
            ImGuiComboFlags flags = 0) override
        {
            return ImGui::BeginCombo(label, preview_value, flags);
        }
        void EndCombo() override
        {
            ImGui::EndCombo();
        }
        bool Combo(const char* label,
            int* current_item,
            const char* const items[],
            int items_count,
            int popup_max_height_in_items = -1) override
        {
            return ImGui::Combo(label,
                current_item,
                items,
                items_count,
                popup_max_height_in_items);
        }
        bool Combo(const char* label,
            int* current_item,
            const char* items_separated_by_zeros,
            int popup_max_height_in_items = -1) override
        {
            return ImGui::Combo(label,
                current_item,
                items_separated_by_zeros,
                popup_max_height_in_items);
        }
        bool Combo(const char* label,
            int* current_item,
            const char* (*getter)(void* user_data, int idx),
            void* user_data,
            int items_count,
            int popup_max_height_in_items = -1) override
        {
            return ImGui::Combo(label,
                current_item,
                getter,
                user_data,
                items_count,
                popup_max_height_in_items);
        }
        bool DragFloat(const char* label,
            float* v,
            float v_speed = 1.0f,
            float v_min = 0.0f,
            float v_max = 0.0f,
            const char* format = "%.3f",
            ImGuiSliderFlags flags = 0) override
        {
            return ImGui::DragFloat(
                label, v, v_speed, v_min, v_max, format, flags);
        }
        bool DragFloat2(const char* label,
            float v[2],
            float v_speed = 1.0f,
            float v_min = 0.0f,
            float v_max = 0.0f,
            const char* format = "%.3f",
            ImGuiSliderFlags flags = 0) override
        {
            return ImGui::DragFloat2(
                label, v, v_speed, v_min, v_max, format, flags);
        }
        bool DragFloat3(const char* label,
            float v[3],
            float v_speed = 1.0f,
            float v_min = 0.0f,
            float v_max = 0.0f,
            const char* format = "%.3f",
            ImGuiSliderFlags flags = 0) override
        {
            return ImGui::DragFloat3(
                label, v, v_speed, v_min, v_max, format, flags);
        }
        bool DragFloat4(const char* label,
            float v[4],
            float v_speed = 1.0f,
            float v_min = 0.0f,
            float v_max = 0.0f,
            const char* format = "%.3f",
            ImGuiSliderFlags flags = 0) override
        {
            return ImGui::DragFloat4(
                label, v, v_speed, v_min, v_max, format, flags);
        }
        bool DragFloatRange2(const char* label,
            float* v_current_min,
            float* v_current_max,
            float v_speed = 1.0f,
            float v_min = 0.0f,
            float v_max = 0.0f,
            const char* format = "%.3f",
            const char* format_max = NULL,
            ImGuiSliderFlags flags = 0) override
        {
            return ImGui::DragFloatRange2(label,
                v_current_min,
                v_current_max,
                v_speed,
                v_min,
                v_max,
                format,
                format_max,
                flags);
        }
        bool DragInt(const char* label,
            int* v,
            float v_speed = 1.0f,
            int v_min = 0,
            int v_max = 0,
            const char* format = "%d",
            ImGuiSliderFlags flags = 0) override
        {
            return ImGui::DragInt(
                label, v, v_speed, v_min, v_max, format, flags);
        }
        bool DragInt2(const char* label,
            int v[2],
            float v_speed = 1.0f,
            int v_min = 0,
            int v_max = 0,
            const char* format = "%d",
            ImGuiSliderFlags flags = 0) override
        {
            return ImGui::DragInt2(
                label, v, v_speed, v_min, v_max, format, flags);
        }
        bool DragInt3(const char* label,
            int v[3],
            float v_speed = 1.0f,
            int v_min = 0,
            int v_max = 0,
            const char* format = "%d",
            ImGuiSliderFlags flags = 0) override
        {
            return ImGui::DragInt3(
                label, v, v_speed, v_min, v_max, format, flags);
        }
        bool DragInt4(const char* label,
            int v[4],
            float v_speed = 1.0f,
            int v_min = 0,
            int v_max = 0,
            const char* format = "%d",
            ImGuiSliderFlags flags = 0) override
        {
            return ImGui::DragInt4(
                label, v, v_speed, v_min, v_max, format, flags);
        }
        bool DragIntRange2(const char* label,
            int* v_current_min,
            int* v_current_max,
            float v_speed = 1.0f,
            int v_min = 0,
            int v_max = 0,
            const char* format = "%d",
            const char* format_max = NULL,
            ImGuiSliderFlags flags = 0) override
        {
            return ImGui::DragIntRange2(label,
                v_current_min,
                v_current_max,
                v_speed,
                v_min,
                v_max,
                format,
                format_max,
                flags);
        }
        bool DragScalar(const char* label,
            ImGuiDataType data_type,
            void* p_data,
            float v_speed = 1.0f,
            const void* p_min = NULL,
            const void* p_max = NULL,
            const char* format = NULL,
            ImGuiSliderFlags flags = 0) override
        {
            return ImGui::DragScalar(
                label, data_type, p_data, v_speed, p_min, p_max, format, flags);
        }
        bool DragScalarN(const char* label,
            ImGuiDataType data_type,
            void* p_data,
            int components,
            float v_speed = 1.0f,
            const void* p_min = NULL,
            const void* p_max = NULL,
            const char* format = NULL,
            ImGuiSliderFlags flags = 0) override
        {
            return ImGui::DragScalarN(label,
                data_type,
                p_data,
                components,
                v_speed,
                p_min,
                p_max,
                format,
                flags);
        }
        bool SliderFloat(const char* label,
            float* v,
            float v_min,
            float v_max,
            const char* format = "%.3f",
            ImGuiSliderFlags flags = 0) override
        {
            return ImGui::SliderFloat(label, v, v_min, v_max, format, flags);
        }
        bool SliderFloat2(const char* label,
            float v[2],
            float v_min,
            float v_max,
            const char* format = "%.3f",
            ImGuiSliderFlags flags = 0) override
        {
            return ImGui::SliderFloat2(label, v, v_min, v_max, format, flags);
        }
        bool SliderFloat3(const char* label,
            float v[3],
            float v_min,
            float v_max,
            const char* format = "%.3f",
            ImGuiSliderFlags flags = 0) override
        {
            return ImGui::SliderFloat3(label, v, v_min, v_max, format, flags);
        }
        bool SliderFloat4(const char* label,
            float v[4],
            float v_min,
            float v_max,
            const char* format = "%.3f",
            ImGuiSliderFlags flags = 0) override
        {
            return ImGui::SliderFloat4(label, v, v_min, v_max, format, flags);
        }
        bool SliderAngle(const char* label,
            float* v_rad,
            float v_degrees_min = -360.0f,
            float v_degrees_max = +360.0f,
            const char* format = "%.0f deg",
            ImGuiSliderFlags flags = 0) override
        {
            return ImGui::SliderAngle(
                label, v_rad, v_degrees_min, v_degrees_max, format, flags);
        }
        bool SliderInt(const char* label,
            int* v,
            int v_min,
            int v_max,
            const char* format = "%d",
            ImGuiSliderFlags flags = 0) override
        {
            return ImGui::SliderInt(label, v, v_min, v_max, format, flags);
        }
        bool SliderInt2(const char* label,
            int v[2],
            int v_min,
            int v_max,
            const char* format = "%d",
            ImGuiSliderFlags flags = 0) override
        {
            return ImGui::SliderInt2(label, v, v_min, v_max, format, flags);
        }
        bool SliderInt3(const char* label,
            int v[3],
            int v_min,
            int v_max,
            const char* format = "%d",
            ImGuiSliderFlags flags = 0) override
        {
            return ImGui::SliderInt3(label, v, v_min, v_max, format, flags);
        }
        bool SliderInt4(const char* label,
            int v[4],
            int v_min,
            int v_max,
            const char* format = "%d",
            ImGuiSliderFlags flags = 0) override
        {
            return ImGui::SliderInt4(label, v, v_min, v_max, format, flags);
        }
        bool SliderScalar(const char* label,
            ImGuiDataType data_type,
            void* p_data,
            const void* p_min,
            const void* p_max,
            const char* format = NULL,
            ImGuiSliderFlags flags = 0) override
        {
            return ImGui::SliderScalar(
                label, data_type, p_data, p_min, p_max, format, flags);
        }
        bool SliderScalarN(const char* label,
            ImGuiDataType data_type,
            void* p_data,
            int components,
            const void* p_min,
            const void* p_max,
            const char* format = NULL,
            ImGuiSliderFlags flags = 0) override
        {
            return ImGui::SliderScalarN(label,
                data_type,
                p_data,
                components,
                p_min,
                p_max,
                format,
                flags);
        }
        bool VSliderFloat(const char* label,
            const ImVec2& size,
            float* v,
            float v_min,
            float v_max,
            const char* format = "%.3f",
            ImGuiSliderFlags flags = 0) override
        {
            return ImGui::VSliderFloat(
                label, size, v, v_min, v_max, format, flags);
        }
        bool VSliderInt(const char* label,
            const ImVec2& size,
            int* v,
            int v_min,
            int v_max,
            const char* format = "%d",
            ImGuiSliderFlags flags = 0) override
        {
            return ImGui::VSliderInt(
                label, size, v, v_min, v_max, format, flags);
        }
        bool VSliderScalar(const char* label,
            const ImVec2& size,
            ImGuiDataType data_type,
            void* p_data,
            const void* p_min,
            const void* p_max,
            const char* format = NULL,
            ImGuiSliderFlags flags = 0) override
        {
            return ImGui::VSliderScalar(
                label, size, data_type, p_data, p_min, p_max, format, flags);
        }
        bool InputText(const char* label,
            char* buf,
            size_t buf_size,
            ImGuiInputTextFlags flags = 0,
            ImGuiInputTextCallback callback = NULL,
            void* user_data = NULL) override
        {
            return ImGui::InputText(
                label, buf, buf_size, flags, callback, user_data);
        }
        bool InputTextMultiline(const char* label,
            char* buf,
            size_t buf_size,
            const ImVec2& size = ImVec2(0, 0),
            ImGuiInputTextFlags flags = 0,
            ImGuiInputTextCallback callback = NULL,
            void* user_data = NULL) override
        {
            return ImGui::InputTextMultiline(
                label, buf, buf_size, size, flags, callback, user_data);
        }
        bool InputTextWithHint(const char* label,
            const char* hint,
            char* buf,
            size_t buf_size,
            ImGuiInputTextFlags flags = 0,
            ImGuiInputTextCallback callback = NULL,
            void* user_data = NULL) override
        {
            return ImGui::InputTextWithHint(
                label, hint, buf, buf_size, flags, callback, user_data);
        }
        bool InputFloat(const char* label,
            float* v,
            float step = 0.0f,
            float step_fast = 0.0f,
            const char* format = "%.3f",
            ImGuiInputTextFlags flags = 0) override
        {
            return ImGui::InputFloat(label, v, step, step_fast, format, flags);
        }
        bool InputFloat2(const char* label,
            float v[2],
            const char* format = "%.3f",
            ImGuiInputTextFlags flags = 0) override
        {
            return ImGui::InputFloat2(label, v, format, flags);
        }
        bool InputFloat3(const char* label,
            float v[3],
            const char* format = "%.3f",
            ImGuiInputTextFlags flags = 0) override
        {
            return ImGui::InputFloat3(label, v, format, flags);
        }
        bool InputFloat4(const char* label,
            float v[4],
            const char* format = "%.3f",
            ImGuiInputTextFlags flags = 0) override
        {
            return ImGui::InputFloat4(label, v, format, flags);
        }
        bool InputInt(const char* label,
            int* v,
            int step = 1,
            int step_fast = 100,
            ImGuiInputTextFlags flags = 0) override
        {
            return ImGui::InputInt(label, v, step, step_fast, flags);
        }
        bool InputInt2(
            const char* label, int v[2], ImGuiInputTextFlags flags = 0) override
        {
            return ImGui::InputInt2(label, v, flags);
        }
        bool InputInt3(
            const char* label, int v[3], ImGuiInputTextFlags flags = 0) override
        {
            return ImGui::InputInt3(label, v, flags);
        }
        bool InputInt4(
            const char* label, int v[4], ImGuiInputTextFlags flags = 0) override
        {
            return ImGui::InputInt4(label, v, flags);
        }
        bool InputDouble(const char* label,
            double* v,
            double step = 0.0,
            double step_fast = 0.0,
            const char* format = "%.6f",
            ImGuiInputTextFlags flags = 0) override
        {
            return ImGui::InputDouble(label, v, step, step_fast, format, flags);
        }
        bool InputScalar(const char* label,
            ImGuiDataType data_type,
            void* p_data,
            const void* p_step = NULL,
            const void* p_step_fast = NULL,
            const char* format = NULL,
            ImGuiInputTextFlags flags = 0) override
        {
            return ImGui::InputScalar(
                label, data_type, p_data, p_step, p_step_fast, format, flags);
        }
        bool InputScalarN(const char* label,
            ImGuiDataType data_type,
            void* p_data,
            int components,
            const void* p_step = NULL,
            const void* p_step_fast = NULL,
            const char* format = NULL,
            ImGuiInputTextFlags flags = 0) override
        {
            return ImGui::InputScalarN(label,
                data_type,
                p_data,
                components,
                p_step,
                p_step_fast,
                format,
                flags);
        }
        bool ColorEdit3(const char* label,
            float col[3],
            ImGuiColorEditFlags flags = 0) override
        {
            return ImGui::ColorEdit3(label, col, flags);
        }
        bool ColorEdit4(const char* label,
            float col[4],
            ImGuiColorEditFlags flags = 0) override
        {
            return ImGui::ColorEdit4(label, col, flags);
        }
        bool ColorPicker3(const char* label,
            float col[3],
            ImGuiColorEditFlags flags = 0) override
        {
            return ImGui::ColorPicker3(label, col, flags);
        }
        bool ColorPicker4(const char* label,
            float col[4],
            ImGuiColorEditFlags flags = 0,
            const float* ref_col = NULL) override
        {
            return ImGui::ColorPicker4(label, col, flags, ref_col);
        }
        bool ColorButton(const char* desc_id,
            const ImVec4& col,
            ImGuiColorEditFlags flags = 0,
            const ImVec2& size = ImVec2(0, 0)) override
        {
            return ImGui::ColorButton(desc_id, col, flags, size);
        }
        bool TreeNode(const char* label) override
        {
            return ImGui::TreeNode(label);
        }
        bool TreeNode(const char* str_id, const char* fmt, ...) override
        {
            va_list args;
            va_start(args, fmt);
            bool _ret = ImGui::TreeNodeV(str_id, fmt, args);
            va_end(args);
            return _ret;
        }
        bool TreeNode(const void* ptr_id, const char* fmt, ...) override
        {
            va_list args;
            va_start(args, fmt);
            bool _ret = ImGui::TreeNodeV(ptr_id, fmt, args);
            va_end(args);
            return _ret;
        }
        bool TreeNodeV(
            const char* str_id, const char* fmt, va_list args) override
        {
            return ImGui::TreeNodeV(str_id, fmt, args);
        }
        bool TreeNodeV(
            const void* ptr_id, const char* fmt, va_list args) override
        {
            return ImGui::TreeNodeV(ptr_id, fmt, args);
        }
        bool TreeNodeEx(
            const char* label, ImGuiTreeNodeFlags flags = 0) override
        {
            return ImGui::TreeNodeEx(label, flags);
        }
        bool TreeNodeEx(const char* str_id,
            ImGuiTreeNodeFlags flags,
            const char* fmt,
            ...) override
        {
            va_list args;
            va_start(args, fmt);
            bool _ret = ImGui::TreeNodeExV(str_id, flags, fmt, args);
            va_end(args);
            return _ret;
        }
        bool TreeNodeEx(const void* ptr_id,
            ImGuiTreeNodeFlags flags,
            const char* fmt,
            ...) override
        {
            va_list args;
            va_start(args, fmt);
            bool _ret = ImGui::TreeNodeExV(ptr_id, flags, fmt, args);
            va_end(args);
            return _ret;
        }
        bool TreeNodeExV(const char* str_id,
            ImGuiTreeNodeFlags flags,
            const char* fmt,
            va_list args) override
        {
            return ImGui::TreeNodeExV(str_id, flags, fmt, args);
        }
        bool TreeNodeExV(const void* ptr_id,
            ImGuiTreeNodeFlags flags,
            const char* fmt,
            va_list args) override
        {
            return ImGui::TreeNodeExV(ptr_id, flags, fmt, args);
        }
        void TreePush(const char* str_id) override
        {
            ImGui::TreePush(str_id);
        }
        void TreePush(const void* ptr_id) override
        {
            ImGui::TreePush(ptr_id);
        }
        void TreePop() override
        {
            ImGui::TreePop();
        }
        float GetTreeNodeToLabelSpacing() override
        {
            return ImGui::GetTreeNodeToLabelSpacing();
        }
        bool CollapsingHeader(
            const char* label, ImGuiTreeNodeFlags flags = 0) override
        {
            return ImGui::CollapsingHeader(label, flags);
        }
        bool CollapsingHeader(const char* label,
            bool* p_visible,
            ImGuiTreeNodeFlags flags = 0) override
        {
            return ImGui::CollapsingHeader(label, p_visible, flags);
        }
        void SetNextItemOpen(bool is_open, ImGuiCond cond = 0) override
        {
            ImGui::SetNextItemOpen(is_open, cond);
        }
        void SetNextItemStorageID(ImGuiID storage_id) override
        {
            ImGui::SetNextItemStorageID(storage_id);
        }
        bool TreeNodeGetOpen(ImGuiID storage_id) override
        {
            return ImGui::TreeNodeGetOpen(storage_id);
        }
        bool Selectable(const char* label,
            bool selected = false,
            ImGuiSelectableFlags flags = 0,
            const ImVec2& size = ImVec2(0, 0)) override
        {
            return ImGui::Selectable(label, selected, flags, size);
        }
        bool Selectable(const char* label,
            bool* p_selected,
            ImGuiSelectableFlags flags = 0,
            const ImVec2& size = ImVec2(0, 0)) override
        {
            return ImGui::Selectable(label, p_selected, flags, size);
        }
        ImGuiMultiSelectIO* BeginMultiSelect(ImGuiMultiSelectFlags flags,
            int selection_size = -1,
            int items_count = -1) override
        {
            return ImGui::BeginMultiSelect(flags, selection_size, items_count);
        }
        ImGuiMultiSelectIO* EndMultiSelect() override
        {
            return ImGui::EndMultiSelect();
        }
        void SetNextItemSelectionUserData(
            ImGuiSelectionUserData selection_user_data) override
        {
            ImGui::SetNextItemSelectionUserData(selection_user_data);
        }
        bool IsItemToggledSelection() override
        {
            return ImGui::IsItemToggledSelection();
        }
        bool BeginListBox(
            const char* label, const ImVec2& size = ImVec2(0, 0)) override
        {
            return ImGui::BeginListBox(label, size);
        }
        void EndListBox() override
        {
            ImGui::EndListBox();
        }
        bool ListBox(const char* label,
            int* current_item,
            const char* const items[],
            int items_count,
            int height_in_items = -1) override
        {
            return ImGui::ListBox(
                label, current_item, items, items_count, height_in_items);
        }
        bool ListBox(const char* label,
            int* current_item,
            const char* (*getter)(void* user_data, int idx),
            void* user_data,
            int items_count,
            int height_in_items = -1) override
        {
            return ImGui::ListBox(label,
                current_item,
                getter,
                user_data,
                items_count,
                height_in_items);
        }
        void PlotLines(const char* label,
            const float* values,
            int values_count,
            int values_offset = 0,
            const char* overlay_text = NULL,
            float scale_min = FLT_MAX,
            float scale_max = FLT_MAX,
            ImVec2 graph_size = ImVec2(0, 0),
            int stride = sizeof(float)) override
        {
            ImGui::PlotLines(label,
                values,
                values_count,
                values_offset,
                overlay_text,
                scale_min,
                scale_max,
                graph_size,
                stride);
        }
        void PlotLines(const char* label,
            float (*values_getter)(void* data, int idx),
            void* data,
            int values_count,
            int values_offset = 0,
            const char* overlay_text = NULL,
            float scale_min = FLT_MAX,
            float scale_max = FLT_MAX,
            ImVec2 graph_size = ImVec2(0, 0)) override
        {
            ImGui::PlotLines(label,
                values_getter,
                data,
                values_count,
                values_offset,
                overlay_text,
                scale_min,
                scale_max,
                graph_size);
        }
        void PlotHistogram(const char* label,
            const float* values,
            int values_count,
            int values_offset = 0,
            const char* overlay_text = NULL,
            float scale_min = FLT_MAX,
            float scale_max = FLT_MAX,
            ImVec2 graph_size = ImVec2(0, 0),
            int stride = sizeof(float)) override
        {
            ImGui::PlotHistogram(label,
                values,
                values_count,
                values_offset,
                overlay_text,
                scale_min,
                scale_max,
                graph_size,
                stride);
        }
        void PlotHistogram(const char* label,
            float (*values_getter)(void* data, int idx),
            void* data,
            int values_count,
            int values_offset = 0,
            const char* overlay_text = NULL,
            float scale_min = FLT_MAX,
            float scale_max = FLT_MAX,
            ImVec2 graph_size = ImVec2(0, 0)) override
        {
            ImGui::PlotHistogram(label,
                values_getter,
                data,
                values_count,
                values_offset,
                overlay_text,
                scale_min,
                scale_max,
                graph_size);
        }
        void Value(const char* prefix, bool b) override
        {
            ImGui::Value(prefix, b);
        }
        void Value(const char* prefix, int v) override
        {
            ImGui::Value(prefix, v);
        }
        void Value(const char* prefix, unsigned int v) override
        {
            ImGui::Value(prefix, v);
        }
        void Value(const char* prefix,
            float v,
            const char* float_format = NULL) override
        {
            ImGui::Value(prefix, v, float_format);
        }
        bool BeginMenuBar() override
        {
            return ImGui::BeginMenuBar();
        }
        void EndMenuBar() override
        {
            ImGui::EndMenuBar();
        }
        bool BeginMainMenuBar() override
        {
            return ImGui::BeginMainMenuBar();
        }
        void EndMainMenuBar() override
        {
            ImGui::EndMainMenuBar();
        }
        bool BeginMenu(const char* label, bool enabled = true) override
        {
            return ImGui::BeginMenu(label, enabled);
        }
        void EndMenu() override
        {
            ImGui::EndMenu();
        }
        bool MenuItem(const char* label,
            const char* shortcut = NULL,
            bool selected = false,
            bool enabled = true) override
        {
            return ImGui::MenuItem(label, shortcut, selected, enabled);
        }
        bool MenuItem(const char* label,
            const char* shortcut,
            bool* p_selected,
            bool enabled = true) override
        {
            return ImGui::MenuItem(label, shortcut, p_selected, enabled);
        }
        bool BeginTooltip() override
        {
            return ImGui::BeginTooltip();
        }
        void EndTooltip() override
        {
            ImGui::EndTooltip();
        }
        void SetTooltip(const char* fmt, ...) override
        {
            va_list args;
            va_start(args, fmt);
            ImGui::SetTooltipV(fmt, args);
            va_end(args);
        }
        void SetTooltipV(const char* fmt, va_list args) override
        {
            ImGui::SetTooltipV(fmt, args);
        }
        bool BeginItemTooltip() override
        {
            return ImGui::BeginItemTooltip();
        }
        void SetItemTooltip(const char* fmt, ...) override
        {
            va_list args;
            va_start(args, fmt);
            ImGui::SetItemTooltipV(fmt, args);
            va_end(args);
        }
        void SetItemTooltipV(const char* fmt, va_list args) override
        {
            ImGui::SetItemTooltipV(fmt, args);
        }
        bool BeginPopup(const char* str_id, ImGuiWindowFlags flags = 0) override
        {
            return ImGui::BeginPopup(str_id, flags);
        }
        bool BeginPopupModal(const char* name,
            bool* p_open = NULL,
            ImGuiWindowFlags flags = 0) override
        {
            return ImGui::BeginPopupModal(name, p_open, flags);
        }
        void EndPopup() override
        {
            ImGui::EndPopup();
        }
        bool OpenPopup(
            const char* str_id, ImGuiPopupFlags popup_flags = 0) override
        {
            return ImGui::OpenPopup(str_id, popup_flags);
        }
        bool OpenPopup(ImGuiID id, ImGuiPopupFlags popup_flags = 0) override
        {
            return ImGui::OpenPopup(id, popup_flags);
        }
        bool OpenPopupOnItemClick(
            const char* str_id = NULL, ImGuiPopupFlags popup_flags = 0) override
        {
            return ImGui::OpenPopupOnItemClick(str_id, popup_flags);
        }
        void CloseCurrentPopup() override
        {
            ImGui::CloseCurrentPopup();
        }
        bool BeginPopupContextItem(
            const char* str_id = NULL, ImGuiPopupFlags popup_flags = 0) override
        {
            return ImGui::BeginPopupContextItem(str_id, popup_flags);
        }
        bool BeginPopupContextWindow(
            const char* str_id = NULL, ImGuiPopupFlags popup_flags = 0) override
        {
            return ImGui::BeginPopupContextWindow(str_id, popup_flags);
        }
        bool BeginPopupContextVoid(
            const char* str_id = NULL, ImGuiPopupFlags popup_flags = 0) override
        {
            return ImGui::BeginPopupContextVoid(str_id, popup_flags);
        }
        bool IsPopupOpen(const char* str_id, ImGuiPopupFlags flags = 0) override
        {
            return ImGui::IsPopupOpen(str_id, flags);
        }
        bool BeginTable(const char* str_id,
            int columns,
            ImGuiTableFlags flags = 0,
            const ImVec2& outer_size = ImVec2(0.0f, 0.0f),
            float inner_width = 0.0f) override
        {
            return ImGui::BeginTable(
                str_id, columns, flags, outer_size, inner_width);
        }
        void EndTable() override
        {
            ImGui::EndTable();
        }
        void TableNextRow(ImGuiTableRowFlags row_flags = 0,
            float min_row_height = 0.0f) override
        {
            ImGui::TableNextRow(row_flags, min_row_height);
        }
        bool TableNextColumn() override
        {
            return ImGui::TableNextColumn();
        }
        bool TableSetColumnIndex(int column_n) override
        {
            return ImGui::TableSetColumnIndex(column_n);
        }
        void TableSetupColumn(const char* label,
            ImGuiTableColumnFlags flags = 0,
            float init_width_or_weight = 0.0f,
            ImGuiID user_data = 0) override
        {
            ImGui::TableSetupColumn(
                label, flags, init_width_or_weight, user_data);
        }
        void TableSetupScrollFreeze(int cols, int rows) override
        {
            ImGui::TableSetupScrollFreeze(cols, rows);
        }
        void TableHeader(const char* label) override
        {
            ImGui::TableHeader(label);
        }
        void TableHeadersRow() override
        {
            ImGui::TableHeadersRow();
        }
        void TableAngledHeadersRow() override
        {
            ImGui::TableAngledHeadersRow();
        }
        ImGuiTableSortSpecs* TableGetSortSpecs() override
        {
            return ImGui::TableGetSortSpecs();
        }
        int TableGetColumnCount() override
        {
            return ImGui::TableGetColumnCount();
        }
        int TableGetColumnIndex() override
        {
            return ImGui::TableGetColumnIndex();
        }
        int TableGetRowIndex() override
        {
            return ImGui::TableGetRowIndex();
        }
        const char* TableGetColumnName(int column_n = -1) override
        {
            return ImGui::TableGetColumnName(column_n);
        }
        ImGuiTableColumnFlags TableGetColumnFlags(int column_n = -1) override
        {
            return ImGui::TableGetColumnFlags(column_n);
        }
        void TableSetColumnEnabled(int column_n, bool v) override
        {
            ImGui::TableSetColumnEnabled(column_n, v);
        }
        int TableGetHoveredColumn() override
        {
            return ImGui::TableGetHoveredColumn();
        }
        void TableSetBgColor(
            ImGuiTableBgTarget target, ImU32 color, int column_n = -1) override
        {
            ImGui::TableSetBgColor(target, color, column_n);
        }
        void Columns(
            int count = 1, const char* id = NULL, bool borders = true) override
        {
            ImGui::Columns(count, id, borders);
        }
        void NextColumn() override
        {
            ImGui::NextColumn();
        }
        int GetColumnIndex() override
        {
            return ImGui::GetColumnIndex();
        }
        float GetColumnWidth(int column_index = -1) override
        {
            return ImGui::GetColumnWidth(column_index);
        }
        void SetColumnWidth(int column_index, float width) override
        {
            ImGui::SetColumnWidth(column_index, width);
        }
        float GetColumnOffset(int column_index = -1) override
        {
            return ImGui::GetColumnOffset(column_index);
        }
        void SetColumnOffset(int column_index, float offset_x) override
        {
            ImGui::SetColumnOffset(column_index, offset_x);
        }
        int GetColumnsCount() override
        {
            return ImGui::GetColumnsCount();
        }
        bool BeginTabBar(
            const char* str_id, ImGuiTabBarFlags flags = 0) override
        {
            return ImGui::BeginTabBar(str_id, flags);
        }
        void EndTabBar() override
        {
            ImGui::EndTabBar();
        }
        bool BeginTabItem(const char* label,
            bool* p_open = NULL,
            ImGuiTabItemFlags flags = 0) override
        {
            return ImGui::BeginTabItem(label, p_open, flags);
        }
        void EndTabItem() override
        {
            ImGui::EndTabItem();
        }
        bool TabItemButton(
            const char* label, ImGuiTabItemFlags flags = 0) override
        {
            return ImGui::TabItemButton(label, flags);
        }
        void SetTabItemClosed(const char* tab_or_docked_window_label) override
        {
            ImGui::SetTabItemClosed(tab_or_docked_window_label);
        }
        ImGuiID DockSpace(ImGuiID dockspace_id,
            const ImVec2& size = ImVec2(0, 0),
            ImGuiDockNodeFlags flags = 0,
            const ImGuiWindowClass* window_class = NULL) override
        {
            return ImGui::DockSpace(dockspace_id, size, flags, window_class);
        }
        ImGuiID DockSpaceOverViewport(ImGuiID dockspace_id = 0,
            const ImGuiViewport* viewport = NULL,
            ImGuiDockNodeFlags flags = 0,
            const ImGuiWindowClass* window_class = NULL) override
        {
            return ImGui::DockSpaceOverViewport(
                dockspace_id, viewport, flags, window_class);
        }
        void SetNextWindowDockID(ImGuiID dock_id, ImGuiCond cond = 0) override
        {
            ImGui::SetNextWindowDockID(dock_id, cond);
        }
        void SetNextWindowClass(const ImGuiWindowClass* window_class) override
        {
            ImGui::SetNextWindowClass(window_class);
        }
        ImGuiID GetWindowDockID() override
        {
            return ImGui::GetWindowDockID();
        }
        bool IsWindowDocked() override
        {
            return ImGui::IsWindowDocked();
        }
        void LogToTTY(int auto_open_depth = -1) override
        {
            ImGui::LogToTTY(auto_open_depth);
        }
        void LogToFile(
            int auto_open_depth = -1, const char* filename = NULL) override
        {
            ImGui::LogToFile(auto_open_depth, filename);
        }
        void LogToClipboard(int auto_open_depth = -1) override
        {
            ImGui::LogToClipboard(auto_open_depth);
        }
        void LogFinish() override
        {
            ImGui::LogFinish();
        }
        void LogButtons() override
        {
            ImGui::LogButtons();
        }
        void LogText(const char* fmt, ...) override
        {
            va_list args;
            va_start(args, fmt);
            ImGui::LogTextV(fmt, args);
            va_end(args);
        }
        void LogTextV(const char* fmt, va_list args) override
        {
            ImGui::LogTextV(fmt, args);
        }
        bool BeginDragDropSource(ImGuiDragDropFlags flags = 0) override
        {
            return ImGui::BeginDragDropSource(flags);
        }
        bool SetDragDropPayload(const char* type,
            const void* data,
            size_t sz,
            ImGuiCond cond = 0) override
        {
            return ImGui::SetDragDropPayload(type, data, sz, cond);
        }
        void EndDragDropSource() override
        {
            ImGui::EndDragDropSource();
        }
        bool BeginDragDropTarget() override
        {
            return ImGui::BeginDragDropTarget();
        }
        const ImGuiPayload* AcceptDragDropPayload(
            const char* type, ImGuiDragDropFlags flags = 0) override
        {
            return ImGui::AcceptDragDropPayload(type, flags);
        }
        void EndDragDropTarget() override
        {
            ImGui::EndDragDropTarget();
        }
        const ImGuiPayload* GetDragDropPayload() override
        {
            return ImGui::GetDragDropPayload();
        }
        void BeginDisabled(bool disabled = true) override
        {
            ImGui::BeginDisabled(disabled);
        }
        void EndDisabled() override
        {
            ImGui::EndDisabled();
        }
        void PushClipRect(const ImVec2& clip_rect_min,
            const ImVec2& clip_rect_max,
            bool intersect_with_current_clip_rect) override
        {
            ImGui::PushClipRect(
                clip_rect_min, clip_rect_max, intersect_with_current_clip_rect);
        }
        void PopClipRect() override
        {
            ImGui::PopClipRect();
        }
        void SetItemDefaultFocus() override
        {
            ImGui::SetItemDefaultFocus();
        }
        void SetKeyboardFocusHere(int offset = 0) override
        {
            ImGui::SetKeyboardFocusHere(offset);
        }
        void SetNavCursorVisible(bool visible) override
        {
            ImGui::SetNavCursorVisible(visible);
        }
        void SetNextItemAllowOverlap() override
        {
            ImGui::SetNextItemAllowOverlap();
        }
        bool IsItemHovered(ImGuiHoveredFlags flags = 0) override
        {
            return ImGui::IsItemHovered(flags);
        }
        bool IsItemActive() override
        {
            return ImGui::IsItemActive();
        }
        bool IsItemFocused() override
        {
            return ImGui::IsItemFocused();
        }
        bool IsItemClicked(ImGuiMouseButton mouse_button = 0) override
        {
            return ImGui::IsItemClicked(mouse_button);
        }
        bool IsItemVisible() override
        {
            return ImGui::IsItemVisible();
        }
        bool IsItemEdited() override
        {
            return ImGui::IsItemEdited();
        }
        bool IsItemActivated() override
        {
            return ImGui::IsItemActivated();
        }
        bool IsItemDeactivated() override
        {
            return ImGui::IsItemDeactivated();
        }
        bool IsItemDeactivatedAfterEdit() override
        {
            return ImGui::IsItemDeactivatedAfterEdit();
        }
        bool IsItemToggledOpen() override
        {
            return ImGui::IsItemToggledOpen();
        }
        bool IsAnyItemHovered() override
        {
            return ImGui::IsAnyItemHovered();
        }
        bool IsAnyItemActive() override
        {
            return ImGui::IsAnyItemActive();
        }
        bool IsAnyItemFocused() override
        {
            return ImGui::IsAnyItemFocused();
        }
        ImGuiID GetItemID() override
        {
            return ImGui::GetItemID();
        }
        ImVec2 GetItemRectMin() override
        {
            return ImGui::GetItemRectMin();
        }
        ImVec2 GetItemRectMax() override
        {
            return ImGui::GetItemRectMax();
        }
        ImVec2 GetItemRectSize() override
        {
            return ImGui::GetItemRectSize();
        }
        ImGuiItemFlags GetItemFlags() override
        {
            return ImGui::GetItemFlags();
        }
        int GetItemClickedCountWithSingleClickDelay(
            ImGuiMouseButton mouse_button = 0, float delay = -1.0f) override
        {
            return ImGui::GetItemClickedCountWithSingleClickDelay(
                mouse_button, delay);
        }
        ImGuiViewport* GetMainViewport() override
        {
            return ImGui::GetMainViewport();
        }
        ImDrawList* GetBackgroundDrawList(
            ImGuiViewport* viewport = NULL) override
        {
            return ImGui::GetBackgroundDrawList(viewport);
        }
        ImDrawList* GetForegroundDrawList(
            ImGuiViewport* viewport = NULL) override
        {
            return ImGui::GetForegroundDrawList(viewport);
        }
        bool IsRectVisible(const ImVec2& size) override
        {
            return ImGui::IsRectVisible(size);
        }
        bool IsRectVisible(
            const ImVec2& rect_min, const ImVec2& rect_max) override
        {
            return ImGui::IsRectVisible(rect_min, rect_max);
        }
        double GetTime() override
        {
            return ImGui::GetTime();
        }
        int GetFrameCount() override
        {
            return ImGui::GetFrameCount();
        }
        ImDrawListSharedData* GetDrawListSharedData() override
        {
            return ImGui::GetDrawListSharedData();
        }
        const char* GetStyleColorName(ImGuiCol idx) override
        {
            return ImGui::GetStyleColorName(idx);
        }
        void SetStateStorage(ImGuiStorage* storage) override
        {
            ImGui::SetStateStorage(storage);
        }
        ImGuiStorage* GetStateStorage() override
        {
            return ImGui::GetStateStorage();
        }
        ImVec2 CalcTextSize(const char* text,
            const char* text_end = NULL,
            bool hide_text_after_double_hash = false,
            float wrap_width = -1.0f) override
        {
            return ImGui::CalcTextSize(
                text, text_end, hide_text_after_double_hash, wrap_width);
        }
        ImVec4 ColorConvertU32ToFloat4(ImU32 in) override
        {
            return ImGui::ColorConvertU32ToFloat4(in);
        }
        ImU32 ColorConvertFloat4ToU32(const ImVec4& in) override
        {
            return ImGui::ColorConvertFloat4ToU32(in);
        }
        void ColorConvertRGBtoHSV(float r,
            float g,
            float b,
            float& out_h,
            float& out_s,
            float& out_v) override
        {
            ImGui::ColorConvertRGBtoHSV(r, g, b, out_h, out_s, out_v);
        }
        void ColorConvertHSVtoRGB(float h,
            float s,
            float v,
            float& out_r,
            float& out_g,
            float& out_b) override
        {
            ImGui::ColorConvertHSVtoRGB(h, s, v, out_r, out_g, out_b);
        }
        bool IsKeyDown(ImGuiKey key) override
        {
            return ImGui::IsKeyDown(key);
        }
        bool IsKeyPressed(ImGuiKey key, bool repeat = true) override
        {
            return ImGui::IsKeyPressed(key, repeat);
        }
        bool IsKeyReleased(ImGuiKey key) override
        {
            return ImGui::IsKeyReleased(key);
        }
        bool IsKeyChordPressed(ImGuiKeyChord key_chord) override
        {
            return ImGui::IsKeyChordPressed(key_chord);
        }
        int GetKeyPressedAmount(
            ImGuiKey key, float repeat_delay, float rate) override
        {
            return ImGui::GetKeyPressedAmount(key, repeat_delay, rate);
        }
        const char* GetKeyName(ImGuiKey key) override
        {
            return ImGui::GetKeyName(key);
        }
        void SetNextFrameWantCaptureKeyboard(
            bool want_capture_keyboard) override
        {
            ImGui::SetNextFrameWantCaptureKeyboard(want_capture_keyboard);
        }
        bool Shortcut(
            ImGuiKeyChord key_chord, ImGuiInputFlags flags = 0) override
        {
            return ImGui::Shortcut(key_chord, flags);
        }
        void SetNextItemShortcut(
            ImGuiKeyChord key_chord, ImGuiInputFlags flags = 0) override
        {
            ImGui::SetNextItemShortcut(key_chord, flags);
        }
        bool SetItemKeyOwner(ImGuiKey key) override
        {
            return ImGui::SetItemKeyOwner(key);
        }
        bool IsMouseDown(ImGuiMouseButton button) override
        {
            return ImGui::IsMouseDown(button);
        }
        bool IsMouseClicked(
            ImGuiMouseButton button, bool repeat = false) override
        {
            return ImGui::IsMouseClicked(button, repeat);
        }
        bool IsMouseReleased(ImGuiMouseButton button) override
        {
            return ImGui::IsMouseReleased(button);
        }
        bool IsMouseDoubleClicked(ImGuiMouseButton button) override
        {
            return ImGui::IsMouseDoubleClicked(button);
        }
        bool IsMouseReleasedWithDelay(
            ImGuiMouseButton button, float delay = -1.f) override
        {
            return ImGui::IsMouseReleasedWithDelay(button, delay);
        }
        int GetMouseClickedCount(ImGuiMouseButton button) override
        {
            return ImGui::GetMouseClickedCount(button);
        }
        bool IsMouseHoveringRect(
            const ImVec2& r_min, const ImVec2& r_max, bool clip = true) override
        {
            return ImGui::IsMouseHoveringRect(r_min, r_max, clip);
        }
        bool IsMousePosValid(const ImVec2* mouse_pos = NULL) override
        {
            return ImGui::IsMousePosValid(mouse_pos);
        }
        bool IsAnyMouseDown() override
        {
            return ImGui::IsAnyMouseDown();
        }
        ImVec2 GetMousePos() override
        {
            return ImGui::GetMousePos();
        }
        ImVec2 GetMousePosOnOpeningCurrentPopup() override
        {
            return ImGui::GetMousePosOnOpeningCurrentPopup();
        }
        bool IsMouseDragging(
            ImGuiMouseButton button, float lock_threshold = -1.0f) override
        {
            return ImGui::IsMouseDragging(button, lock_threshold);
        }
        ImVec2 GetMouseDragDelta(
            ImGuiMouseButton button = 0, float lock_threshold = -1.0f) override
        {
            return ImGui::GetMouseDragDelta(button, lock_threshold);
        }
        void ResetMouseDragDelta(ImGuiMouseButton button = 0) override
        {
            ImGui::ResetMouseDragDelta(button);
        }
        ImGuiMouseCursor GetMouseCursor() override
        {
            return ImGui::GetMouseCursor();
        }
        void SetMouseCursor(ImGuiMouseCursor cursor_type) override
        {
            ImGui::SetMouseCursor(cursor_type);
        }
        void SetNextFrameWantCaptureMouse(bool want_capture_mouse) override
        {
            ImGui::SetNextFrameWantCaptureMouse(want_capture_mouse);
        }
        const char* GetClipboardText() override
        {
            return ImGui::GetClipboardText();
        }
        void SetClipboardText(const char* text) override
        {
            ImGui::SetClipboardText(text);
        }
        void LoadIniSettingsFromDisk(const char* ini_filename) override
        {
            ImGui::LoadIniSettingsFromDisk(ini_filename);
        }
        void LoadIniSettingsFromMemory(
            const char* ini_data, size_t ini_size = 0) override
        {
            ImGui::LoadIniSettingsFromMemory(ini_data, ini_size);
        }
        void SaveIniSettingsToDisk(const char* ini_filename) override
        {
            ImGui::SaveIniSettingsToDisk(ini_filename);
        }
        const char* SaveIniSettingsToMemory(
            size_t* out_ini_size = NULL) override
        {
            return ImGui::SaveIniSettingsToMemory(out_ini_size);
        }
        void DebugTextEncoding(const char* text) override
        {
            ImGui::DebugTextEncoding(text);
        }
        void DebugFlashStyleColor(ImGuiCol idx) override
        {
            ImGui::DebugFlashStyleColor(idx);
        }
        void DebugStartItemPicker() override
        {
            ImGui::DebugStartItemPicker();
        }
        bool DebugCheckVersionAndDataLayout(const char* version_str,
            size_t sz_io,
            size_t sz_style,
            size_t sz_vec2,
            size_t sz_vec4,
            size_t sz_drawvert,
            size_t sz_drawidx) override
        {
            return ImGui::DebugCheckVersionAndDataLayout(version_str,
                sz_io,
                sz_style,
                sz_vec2,
                sz_vec4,
                sz_drawvert,
                sz_drawidx);
        }
        void DebugLog(const char* fmt, ...) override
        {
            va_list args;
            va_start(args, fmt);
            ImGui::DebugLogV(fmt, args);
            va_end(args);
        }
        void DebugLogV(const char* fmt, va_list args) override
        {
            ImGui::DebugLogV(fmt, args);
        }
        void SetAllocatorFunctions(ImGuiMemAllocFunc alloc_func,
            ImGuiMemFreeFunc free_func,
            void* user_data = NULL) override
        {
            ImGui::SetAllocatorFunctions(alloc_func, free_func, user_data);
        }
        void GetAllocatorFunctions(ImGuiMemAllocFunc* p_alloc_func,
            ImGuiMemFreeFunc* p_free_func,
            void** p_user_data) override
        {
            ImGui::GetAllocatorFunctions(
                p_alloc_func, p_free_func, p_user_data);
        }
        void* MemAlloc(size_t size) override
        {
            return ImGui::MemAlloc(size);
        }
        void MemFree(void* ptr) override
        {
            ImGui::MemFree(ptr);
        }
        void UpdatePlatformWindows() override
        {
            ImGui::UpdatePlatformWindows();
        }
        void RenderPlatformWindowsDefault(void* platform_render_arg = NULL,
            void* renderer_render_arg = NULL) override
        {
            ImGui::RenderPlatformWindowsDefault(
                platform_render_arg, renderer_render_arg);
        }
        void DestroyPlatformWindows() override
        {
            ImGui::DestroyPlatformWindows();
        }
        ImGuiViewport* FindViewportByID(ImGuiID viewport_id) override
        {
            return ImGui::FindViewportByID(viewport_id);
        }
        ImGuiViewport* FindViewportByPlatformHandle(
            void* platform_handle) override
        {
            return ImGui::FindViewportByPlatformHandle(platform_handle);
        }
        ImPlotContext* ImPlot_CreateContext() override
        {
            return ImPlot::CreateContext();
        }
        void ImPlot_DestroyContext(ImPlotContext* ctx = nullptr) override
        {
            ImPlot::DestroyContext(ctx);
        }
        ImPlotContext* ImPlot_GetCurrentContext() override
        {
            return ImPlot::GetCurrentContext();
        }
        void ImPlot_SetCurrentContext(ImPlotContext* ctx) override
        {
            ImPlot::SetCurrentContext(ctx);
        }
        void SetImGuiContext(ImGuiContext* ctx) override
        {
            ImPlot::SetImGuiContext(ctx);
        }
        bool BeginPlot(const char* title_id,
            const ImVec2& size = ImVec2(-1, 0),
            ImPlotFlags flags = 0) override
        {
            return ImPlot::BeginPlot(title_id, size, flags);
        }
        void EndPlot() override
        {
            ImPlot::EndPlot();
        }
        bool BeginSubplots(const char* title_id,
            int rows,
            int cols,
            const ImVec2& size,
            ImPlotSubplotFlags flags = 0,
            float* row_ratios = nullptr,
            float* col_ratios = nullptr) override
        {
            return ImPlot::BeginSubplots(
                title_id, rows, cols, size, flags, row_ratios, col_ratios);
        }
        void EndSubplots() override
        {
            ImPlot::EndSubplots();
        }
        void SetupAxis(ImAxis axis,
            const char* label = nullptr,
            ImPlotAxisFlags flags = 0) override
        {
            ImPlot::SetupAxis(axis, label, flags);
        }
        void SetupAxisLimits(ImAxis axis,
            double v_min,
            double v_max,
            ImPlotCond cond = ImPlotCond_Once) override
        {
            ImPlot::SetupAxisLimits(axis, v_min, v_max, cond);
        }
        void SetupAxisLinks(
            ImAxis axis, double* link_min, double* link_max) override
        {
            ImPlot::SetupAxisLinks(axis, link_min, link_max);
        }
        void SetupAxisFormat(ImAxis axis, const char* fmt) override
        {
            ImPlot::SetupAxisFormat(axis, fmt);
        }
        void SetupAxisFormat(ImAxis axis,
            ImPlotFormatter formatter,
            void* data = nullptr) override
        {
            ImPlot::SetupAxisFormat(axis, formatter, data);
        }
        void SetupAxisTicks(ImAxis axis,
            const double* values,
            int n_ticks,
            const char* const labels[] = nullptr,
            bool keep_default = false) override
        {
            ImPlot::SetupAxisTicks(axis, values, n_ticks, labels, keep_default);
        }
        void SetupAxisTicks(ImAxis axis,
            double v_min,
            double v_max,
            int n_ticks,
            const char* const labels[] = nullptr,
            bool keep_default = false) override
        {
            ImPlot::SetupAxisTicks(
                axis, v_min, v_max, n_ticks, labels, keep_default);
        }
        void SetupAxisScale(ImAxis axis, ImPlotScale scale) override
        {
            ImPlot::SetupAxisScale(axis, scale);
        }
        void SetupAxisScale(ImAxis axis,
            ImPlotTransform forward,
            ImPlotTransform inverse,
            void* data = nullptr) override
        {
            ImPlot::SetupAxisScale(axis, forward, inverse, data);
        }
        void SetupAxisLimitsConstraints(
            ImAxis axis, double v_min, double v_max) override
        {
            ImPlot::SetupAxisLimitsConstraints(axis, v_min, v_max);
        }
        void SetupAxisZoomConstraints(
            ImAxis axis, double z_min, double z_max) override
        {
            ImPlot::SetupAxisZoomConstraints(axis, z_min, z_max);
        }
        void SetupAxes(const char* x_label,
            const char* y_label,
            ImPlotAxisFlags x_flags = 0,
            ImPlotAxisFlags y_flags = 0) override
        {
            ImPlot::SetupAxes(x_label, y_label, x_flags, y_flags);
        }
        void SetupAxesLimits(double x_min,
            double x_max,
            double y_min,
            double y_max,
            ImPlotCond cond = ImPlotCond_Once) override
        {
            ImPlot::SetupAxesLimits(x_min, x_max, y_min, y_max, cond);
        }
        void SetupLegend(
            ImPlotLocation location, ImPlotLegendFlags flags = 0) override
        {
            ImPlot::SetupLegend(location, flags);
        }
        void SetupMouseText(
            ImPlotLocation location, ImPlotMouseTextFlags flags = 0) override
        {
            ImPlot::SetupMouseText(location, flags);
        }
        void SetupFinish() override
        {
            ImPlot::SetupFinish();
        }
        void SetNextAxisLimits(ImAxis axis,
            double v_min,
            double v_max,
            ImPlotCond cond = ImPlotCond_Once) override
        {
            ImPlot::SetNextAxisLimits(axis, v_min, v_max, cond);
        }
        void SetNextAxisLinks(
            ImAxis axis, double* link_min, double* link_max) override
        {
            ImPlot::SetNextAxisLinks(axis, link_min, link_max);
        }
        void SetNextAxisToFit(ImAxis axis) override
        {
            ImPlot::SetNextAxisToFit(axis);
        }
        void SetNextAxesLimits(double x_min,
            double x_max,
            double y_min,
            double y_max,
            ImPlotCond cond = ImPlotCond_Once) override
        {
            ImPlot::SetNextAxesLimits(x_min, x_max, y_min, y_max, cond);
        }
        void SetNextAxesToFit() override
        {
            ImPlot::SetNextAxesToFit();
        }
        void PlotLineG(const char* label_id,
            ImPlotGetter getter,
            void* data,
            int count,
            const ImPlotSpec& spec = ImPlotSpec()) override
        {
            ImPlot::PlotLineG(label_id, getter, data, count, spec);
        }
        void PlotScatterG(const char* label_id,
            ImPlotGetter getter,
            void* data,
            int count,
            const ImPlotSpec& spec = ImPlotSpec()) override
        {
            ImPlot::PlotScatterG(label_id, getter, data, count, spec);
        }
        void PlotStairsG(const char* label_id,
            ImPlotGetter getter,
            void* data,
            int count,
            const ImPlotSpec& spec = ImPlotSpec()) override
        {
            ImPlot::PlotStairsG(label_id, getter, data, count, spec);
        }
        void PlotShadedG(const char* label_id,
            ImPlotGetter getter1,
            void* data1,
            ImPlotGetter getter2,
            void* data2,
            int count,
            const ImPlotSpec& spec = ImPlotSpec()) override
        {
            ImPlot::PlotShadedG(
                label_id, getter1, data1, getter2, data2, count, spec);
        }
        void PlotBarsG(const char* label_id,
            ImPlotGetter getter,
            void* data,
            int count,
            double bar_size,
            const ImPlotSpec& spec = ImPlotSpec()) override
        {
            ImPlot::PlotBarsG(label_id, getter, data, count, bar_size, spec);
        }
        void PlotDigitalG(const char* label_id,
            ImPlotGetter getter,
            void* data,
            int count,
            const ImPlotSpec& spec = ImPlotSpec()) override
        {
            ImPlot::PlotDigitalG(label_id, getter, data, count, spec);
        }
        void PlotImage(const char* label_id,
            ImTextureRef tex_ref,
            const ImPlotPoint& bounds_min,
            const ImPlotPoint& bounds_max,
            const ImVec2& uv0 = ImVec2(0, 0),
            const ImVec2& uv1 = ImVec2(1, 1),
            const ImVec4& tint_col = ImVec4(1, 1, 1, 1),
            const ImPlotSpec& spec = ImPlotSpec()) override
        {
            ImPlot::PlotImage(label_id,
                tex_ref,
                bounds_min,
                bounds_max,
                uv0,
                uv1,
                tint_col,
                spec);
        }
        void PlotImage(const char* label_id,
            ImTextureID tex_ref,
            const ImPlotPoint& bounds_min,
            const ImPlotPoint& bounds_max,
            const ImVec2& uv0 = ImVec2(0, 0),
            const ImVec2& uv1 = ImVec2(1, 1),
            const ImVec4& tint_col = ImVec4(1, 1, 1, 1),
            const ImPlotSpec& spec = ImPlotSpec()) override
        {
            ImPlot::PlotImage(label_id,
                tex_ref,
                bounds_min,
                bounds_max,
                uv0,
                uv1,
                tint_col,
                spec);
        }
        void PlotText(const char* text,
            double x,
            double y,
            const ImVec2& pix_offset = ImVec2(0, 0),
            const ImPlotSpec& spec = ImPlotSpec()) override
        {
            ImPlot::PlotText(text, x, y, pix_offset, spec);
        }
        void PlotDummy(const char* label_id,
            const ImPlotSpec& spec = ImPlotSpec()) override
        {
            ImPlot::PlotDummy(label_id, spec);
        }
        bool DragPoint(int id,
            double* x,
            double* y,
            const ImVec4& col,
            float size = 4,
            ImPlotDragToolFlags flags = 0,
            bool* out_clicked = nullptr,
            bool* out_hovered = nullptr,
            bool* out_held = nullptr) override
        {
            return ImPlot::DragPoint(
                id, x, y, col, size, flags, out_clicked, out_hovered, out_held);
        }
        bool DragLineX(int id,
            double* x,
            const ImVec4& col,
            float thickness = 1,
            ImPlotDragToolFlags flags = 0,
            bool* out_clicked = nullptr,
            bool* out_hovered = nullptr,
            bool* out_held = nullptr) override
        {
            return ImPlot::DragLineX(id,
                x,
                col,
                thickness,
                flags,
                out_clicked,
                out_hovered,
                out_held);
        }
        bool DragLineY(int id,
            double* y,
            const ImVec4& col,
            float thickness = 1,
            ImPlotDragToolFlags flags = 0,
            bool* out_clicked = nullptr,
            bool* out_hovered = nullptr,
            bool* out_held = nullptr) override
        {
            return ImPlot::DragLineY(id,
                y,
                col,
                thickness,
                flags,
                out_clicked,
                out_hovered,
                out_held);
        }
        bool DragRect(int id,
            double* x1,
            double* y1,
            double* x2,
            double* y2,
            const ImVec4& col,
            ImPlotDragToolFlags flags = 0,
            bool* out_clicked = nullptr,
            bool* out_hovered = nullptr,
            bool* out_held = nullptr) override
        {
            return ImPlot::DragRect(id,
                x1,
                y1,
                x2,
                y2,
                col,
                flags,
                out_clicked,
                out_hovered,
                out_held);
        }
        void Annotation(double x,
            double y,
            const ImVec4& col,
            const ImVec2& pix_offset,
            bool clamp,
            bool round = false) override
        {
            ImPlot::Annotation(x, y, col, pix_offset, clamp, round);
        }
        void Annotation(double x,
            double y,
            const ImVec4& col,
            const ImVec2& pix_offset,
            bool clamp,
            const char* fmt,
            ...) override
        {
            va_list args;
            va_start(args, fmt);
            ImPlot::AnnotationV(x, y, col, pix_offset, clamp, fmt, args);
            va_end(args);
        }
        void AnnotationV(double x,
            double y,
            const ImVec4& col,
            const ImVec2& pix_offset,
            bool clamp,
            const char* fmt,
            va_list args) override
        {
            ImPlot::AnnotationV(x, y, col, pix_offset, clamp, fmt, args);
        }
        void TagX(double x, const ImVec4& col, bool round = false) override
        {
            ImPlot::TagX(x, col, round);
        }
        void TagX(double x, const ImVec4& col, const char* fmt, ...) override
        {
            va_list args;
            va_start(args, fmt);
            ImPlot::TagXV(x, col, fmt, args);
            va_end(args);
        }
        void TagXV(
            double x, const ImVec4& col, const char* fmt, va_list args) override
        {
            ImPlot::TagXV(x, col, fmt, args);
        }
        void TagY(double y, const ImVec4& col, bool round = false) override
        {
            ImPlot::TagY(y, col, round);
        }
        void TagY(double y, const ImVec4& col, const char* fmt, ...) override
        {
            va_list args;
            va_start(args, fmt);
            ImPlot::TagYV(y, col, fmt, args);
            va_end(args);
        }
        void TagYV(
            double y, const ImVec4& col, const char* fmt, va_list args) override
        {
            ImPlot::TagYV(y, col, fmt, args);
        }
        void SetAxis(ImAxis axis) override
        {
            ImPlot::SetAxis(axis);
        }
        void SetAxes(ImAxis x_axis, ImAxis y_axis) override
        {
            ImPlot::SetAxes(x_axis, y_axis);
        }
        ImPlotPoint PixelsToPlot(const ImVec2& pix,
            ImAxis x_axis = IMPLOT_AUTO,
            ImAxis y_axis = IMPLOT_AUTO) override
        {
            return ImPlot::PixelsToPlot(pix, x_axis, y_axis);
        }
        ImPlotPoint PixelsToPlot(float x,
            float y,
            ImAxis x_axis = IMPLOT_AUTO,
            ImAxis y_axis = IMPLOT_AUTO) override
        {
            return ImPlot::PixelsToPlot(x, y, x_axis, y_axis);
        }
        ImVec2 PlotToPixels(const ImPlotPoint& plt,
            ImAxis x_axis = IMPLOT_AUTO,
            ImAxis y_axis = IMPLOT_AUTO) override
        {
            return ImPlot::PlotToPixels(plt, x_axis, y_axis);
        }
        ImVec2 PlotToPixels(double x,
            double y,
            ImAxis x_axis = IMPLOT_AUTO,
            ImAxis y_axis = IMPLOT_AUTO) override
        {
            return ImPlot::PlotToPixels(x, y, x_axis, y_axis);
        }
        ImVec2 GetPlotPos() override
        {
            return ImPlot::GetPlotPos();
        }
        ImVec2 GetPlotSize() override
        {
            return ImPlot::GetPlotSize();
        }
        ImPlotPoint GetPlotMousePos(
            ImAxis x_axis = IMPLOT_AUTO, ImAxis y_axis = IMPLOT_AUTO) override
        {
            return ImPlot::GetPlotMousePos(x_axis, y_axis);
        }
        ImPlotRect GetPlotLimits(
            ImAxis x_axis = IMPLOT_AUTO, ImAxis y_axis = IMPLOT_AUTO) override
        {
            return ImPlot::GetPlotLimits(x_axis, y_axis);
        }
        bool IsPlotHovered() override
        {
            return ImPlot::IsPlotHovered();
        }
        bool IsAxisHovered(ImAxis axis) override
        {
            return ImPlot::IsAxisHovered(axis);
        }
        bool IsSubplotsHovered() override
        {
            return ImPlot::IsSubplotsHovered();
        }
        bool IsPlotSelected() override
        {
            return ImPlot::IsPlotSelected();
        }
        ImPlotRect GetPlotSelection(
            ImAxis x_axis = IMPLOT_AUTO, ImAxis y_axis = IMPLOT_AUTO) override
        {
            return ImPlot::GetPlotSelection(x_axis, y_axis);
        }
        void CancelPlotSelection() override
        {
            ImPlot::CancelPlotSelection();
        }
        void HideNextItem(
            bool hidden = true, ImPlotCond cond = ImPlotCond_Once) override
        {
            ImPlot::HideNextItem(hidden, cond);
        }
        bool BeginAlignedPlots(
            const char* group_id, bool vertical = true) override
        {
            return ImPlot::BeginAlignedPlots(group_id, vertical);
        }
        void EndAlignedPlots() override
        {
            ImPlot::EndAlignedPlots();
        }
        bool BeginLegendPopup(
            const char* label_id, ImGuiMouseButton mouse_button = 1) override
        {
            return ImPlot::BeginLegendPopup(label_id, mouse_button);
        }
        void EndLegendPopup() override
        {
            ImPlot::EndLegendPopup();
        }
        bool IsLegendEntryHovered(const char* label_id) override
        {
            return ImPlot::IsLegendEntryHovered(label_id);
        }
        bool BeginDragDropTargetPlot() override
        {
            return ImPlot::BeginDragDropTargetPlot();
        }
        bool BeginDragDropTargetAxis(ImAxis axis) override
        {
            return ImPlot::BeginDragDropTargetAxis(axis);
        }
        bool BeginDragDropTargetLegend() override
        {
            return ImPlot::BeginDragDropTargetLegend();
        }
        void ImPlot_EndDragDropTarget() override
        {
            ImPlot::EndDragDropTarget();
        }
        bool BeginDragDropSourcePlot(ImGuiDragDropFlags flags = 0) override
        {
            return ImPlot::BeginDragDropSourcePlot(flags);
        }
        bool BeginDragDropSourceAxis(
            ImAxis axis, ImGuiDragDropFlags flags = 0) override
        {
            return ImPlot::BeginDragDropSourceAxis(axis, flags);
        }
        bool BeginDragDropSourceItem(
            const char* label_id, ImGuiDragDropFlags flags = 0) override
        {
            return ImPlot::BeginDragDropSourceItem(label_id, flags);
        }
        void ImPlot_EndDragDropSource() override
        {
            ImPlot::EndDragDropSource();
        }
        ImPlotStyle& ImPlot_GetStyle() override
        {
            return ImPlot::GetStyle();
        }
        void StyleColorsAuto(ImPlotStyle* dst = nullptr) override
        {
            ImPlot::StyleColorsAuto(dst);
        }
        void ImPlot_StyleColorsClassic(ImPlotStyle* dst = nullptr) override
        {
            ImPlot::StyleColorsClassic(dst);
        }
        void ImPlot_StyleColorsDark(ImPlotStyle* dst = nullptr) override
        {
            ImPlot::StyleColorsDark(dst);
        }
        void ImPlot_StyleColorsLight(ImPlotStyle* dst = nullptr) override
        {
            ImPlot::StyleColorsLight(dst);
        }
        void ImPlot_PushStyleColor(ImPlotCol idx, ImU32 col) override
        {
            ImPlot::PushStyleColor(idx, col);
        }
        void ImPlot_PushStyleColor(ImPlotCol idx, const ImVec4& col) override
        {
            ImPlot::PushStyleColor(idx, col);
        }
        void ImPlot_PopStyleColor(int count = 1) override
        {
            ImPlot::PopStyleColor(count);
        }
        void ImPlot_PushStyleVar(ImPlotStyleVar idx, float val) override
        {
            ImPlot::PushStyleVar(idx, val);
        }
        void ImPlot_PushStyleVar(ImPlotStyleVar idx, int val) override
        {
            ImPlot::PushStyleVar(idx, val);
        }
        void ImPlot_PushStyleVar(ImPlotStyleVar idx, const ImVec2& val) override
        {
            ImPlot::PushStyleVar(idx, val);
        }
        void ImPlot_PopStyleVar(int count = 1) override
        {
            ImPlot::PopStyleVar(count);
        }
        ImVec4 GetLastItemColor() override
        {
            return ImPlot::GetLastItemColor();
        }
        const char* ImPlot_GetStyleColorName(ImPlotCol idx) override
        {
            return ImPlot::GetStyleColorName(idx);
        }
        const char* GetMarkerName(ImPlotMarker idx) override
        {
            return ImPlot::GetMarkerName(idx);
        }
        ImPlotMarker NextMarker() override
        {
            return ImPlot::NextMarker();
        }
        ImPlotColormap AddColormap(const char* name,
            const ImVec4* cols,
            int size,
            bool qual = true) override
        {
            return ImPlot::AddColormap(name, cols, size, qual);
        }
        ImPlotColormap AddColormap(const char* name,
            const ImU32* cols,
            int size,
            bool qual = true) override
        {
            return ImPlot::AddColormap(name, cols, size, qual);
        }
        int GetColormapCount() override
        {
            return ImPlot::GetColormapCount();
        }
        const char* GetColormapName(ImPlotColormap cmap) override
        {
            return ImPlot::GetColormapName(cmap);
        }
        ImPlotColormap GetColormapIndex(const char* name) override
        {
            return ImPlot::GetColormapIndex(name);
        }
        void PushColormap(ImPlotColormap cmap) override
        {
            ImPlot::PushColormap(cmap);
        }
        void PushColormap(const char* name) override
        {
            ImPlot::PushColormap(name);
        }
        void PopColormap(int count = 1) override
        {
            ImPlot::PopColormap(count);
        }
        ImVec4 NextColormapColor() override
        {
            return ImPlot::NextColormapColor();
        }
        int GetColormapSize(ImPlotColormap cmap = IMPLOT_AUTO) override
        {
            return ImPlot::GetColormapSize(cmap);
        }
        ImVec4 GetColormapColor(
            int idx, ImPlotColormap cmap = IMPLOT_AUTO) override
        {
            return ImPlot::GetColormapColor(idx, cmap);
        }
        ImVec4 SampleColormap(
            float t, ImPlotColormap cmap = IMPLOT_AUTO) override
        {
            return ImPlot::SampleColormap(t, cmap);
        }
        void ColormapScale(const char* label,
            double scale_min,
            double scale_max,
            const ImVec2& size = ImVec2(0, 0),
            const char* format = "%g",
            ImPlotColormapScaleFlags flags = 0,
            ImPlotColormap cmap = IMPLOT_AUTO) override
        {
            ImPlot::ColormapScale(
                label, scale_min, scale_max, size, format, flags, cmap);
        }
        bool ColormapSlider(const char* label,
            float* t,
            ImVec4* out = nullptr,
            const char* format = "",
            ImPlotColormap cmap = IMPLOT_AUTO) override
        {
            return ImPlot::ColormapSlider(label, t, out, format, cmap);
        }
        bool ColormapButton(const char* label,
            const ImVec2& size = ImVec2(0, 0),
            ImPlotColormap cmap = IMPLOT_AUTO) override
        {
            return ImPlot::ColormapButton(label, size, cmap);
        }
        void BustColorCache(const char* plot_title_id = nullptr) override
        {
            ImPlot::BustColorCache(plot_title_id);
        }
        ImPlotInputMap& GetInputMap() override
        {
            return ImPlot::GetInputMap();
        }
        void MapInputDefault(ImPlotInputMap* dst = nullptr) override
        {
            ImPlot::MapInputDefault(dst);
        }
        void MapInputReverse(ImPlotInputMap* dst = nullptr) override
        {
            ImPlot::MapInputReverse(dst);
        }
        void ItemIcon(const ImVec4& col) override
        {
            ImPlot::ItemIcon(col);
        }
        void ItemIcon(ImU32 col) override
        {
            ImPlot::ItemIcon(col);
        }
        void ColormapIcon(ImPlotColormap cmap) override
        {
            ImPlot::ColormapIcon(cmap);
        }
        ImDrawList* GetPlotDrawList() override
        {
            return ImPlot::GetPlotDrawList();
        }
        void PushPlotClipRect(float expand = 0) override
        {
            ImPlot::PushPlotClipRect(expand);
        }
        void PopPlotClipRect() override
        {
            ImPlot::PopPlotClipRect();
        }
        bool ImPlot_ShowStyleSelector(const char* label) override
        {
            return ImPlot::ShowStyleSelector(label);
        }
        bool ShowColormapSelector(const char* label) override
        {
            return ImPlot::ShowColormapSelector(label);
        }
        bool ShowInputMapSelector(const char* label) override
        {
            return ImPlot::ShowInputMapSelector(label);
        }
        void ImPlot_ShowStyleEditor(ImPlotStyle* ref = nullptr) override
        {
            // ImPlot::ShowStyleEditor(ref);
        }
        void ImPlot_ShowUserGuide() override
        {
            // ImPlot::ShowUserGuide();
        }
        void ImPlot_ShowMetricsWindow(bool* p_popen = nullptr) override
        {
            // ImPlot::ShowMetricsWindow(p_popen);
        }
        void ImPlot_ShowDemoWindow(bool* p_open = nullptr) override
        {
            (void)p_open;
        }
        void SetColorEditOptions(ImGuiColorEditFlags flags) override
        {
            ImGui::SetColorEditOptions(flags);
        }
        void SetWindowFontScale(float scale) override
        {
            ImGui::SetWindowFontScale(scale);
        }
        void Image(ImTextureRef tex_ref,
            const ImVec2& image_size,
            const ImVec2& uv0,
            const ImVec2& uv1,
            const ImVec4& tint_col,
            const ImVec4& border_col) override
        {
            ImGui::Image(tex_ref, image_size, uv0, uv1, tint_col, border_col);
        }
        ImVec2 GetContentRegionMax() override
        {
            return ImGui::GetContentRegionMax();
        }
        ImVec2 GetWindowContentRegionMin() override
        {
            return ImGui::GetWindowContentRegionMin();
        }
        ImVec2 GetWindowContentRegionMax() override
        {
            return ImGui::GetWindowContentRegionMax();
        }
    };

} // namespace

std::unique_ptr<ImGuiIf> CreateImGui()
{
    return std::make_unique<ImGuiIfImpl>();
}
