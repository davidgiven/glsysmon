#include <imgui.h>
#include "dep/codicons/codicons.h"

#include <algorithm>
#include <cstdio>
#include <csetjmp>

#include "configuration.h"
#include "app.h"
#include "imguiif.h"
#include "imgui_helpers.h"
#include "preferences/preferences.h"
#include "globals.h"
#include "sensors/sensor.h"
#include "views/views.h"

jmp_buf g_restartJmp;

ConfigurationWindow::ConfigurationWindow(
    const Views& views, App& app, ImGuiIf& imgui):
    _views(views),
    _app(app),
    _imgui(imgui),
    _pendingMapPreferences(CreateMapPreferences()),
    _pendingPreferences(CreateCombinedPreferences(
        {_pendingMapPreferences, app.GetPreferences()}))
{
}

void ConfigurationWindow::DrawGlobalConfiguration()
{
    // Panel width
    int size = _pendingPreferences->GetInteger("size").value_or(100);
    if (_imgui.InputInt("Width", &size))
    {
        _pendingPreferences->SetInteger("size", size);
    }

    // Dock side
    static constexpr const char* kSideLabels[] = {"left", "right"};
    static constexpr const char* kSideValues[] = {"left", "right"};
    std::string currentSide =
        _pendingPreferences->GetString("side").value_or("left");
    int sideIndex = indexOf(kSideValues, currentSide).value_or(0);

    if (_imgui.SliderInt("Side", &sideIndex, 0, 1, kSideLabels[sideIndex]))
    {
        _pendingPreferences->SetString("side", kSideValues[sideIndex]);
    }

    // Font size
    int fontSize = _pendingPreferences->GetInteger("font_size").value_or(16);
    if (_imgui.InputInt("Font size", &fontSize))
    {
        _pendingPreferences->SetInteger("font_size", fontSize);
    }

    // Small font scale factor (for labels in graphs)
    double scale =
        GlobalPreferencesFetcher::GetSmallFontScale(*_pendingPreferences);
    if (_imgui.InputDouble("Small font scale", &scale))
    {
        _pendingPreferences->SetDouble("small_font_scale", scale);
    }
}

void ConfigurationWindow::Draw(bool* open)
{
    float buttonHeight = _imgui.GetFrameHeightWithSpacing();

    if (_imgui.BeginChild("UpperArea", ImVec2(0, -buttonHeight), false))
    {
        _imgui.SetNextItemOpen(_openSection == 0);
        bool globalOpen = _imgui.CollapsingHeader("Global");
        if (globalOpen != (_openSection == 0))
        {
            _openSection = globalOpen ? 0 : -1;
        }
        if (_openSection == 0)
        {
            float indent = _imgui.GetFontSize() * 2.0f;
            _imgui.Indent(indent);
            DrawGlobalConfiguration();
            _imgui.Unindent(indent);
        }

        std::vector<std::string> viewOrder =
            GlobalPreferencesFetcher::GetViews(*_pendingPreferences);
        {
            std::set<std::string> seen(viewOrder.begin(), viewOrder.end());
            for (const std::string& name : _views.GetAvailableNames())
                if (!seen.contains(name))
                    viewOrder.push_back(name);
        }
        viewOrder.erase(std::remove_if(viewOrder.begin(),
                            viewOrder.end(),
                            [&](const std::string& name)
                            {
                                return _views.Get(name) == nullptr;
                            }),
            viewOrder.end());
        _pendingPreferences->SetStringList("views", viewOrder);

        for (size_t i = 0; i < viewOrder.size(); ++i)
        {
            View* view = _views.Get(viewOrder[i]);
            _imgui.PushID(view->GetPrefName().c_str());
            std::string enabledKey = view->GetPrefName() + ".enabled";
            bool enabled =
                _pendingPreferences->GetBoolean(enabledKey).value_or(true);
            int section = static_cast<int>(i) + 1;
            _imgui.SetNextItemOpen(_openSection == section);
            bool isOpen = _imgui.CollapsingHeader(
                view->GetHumanName().c_str(), ImGuiTreeNodeFlags_AllowOverlap);
            if (isOpen != (_openSection == section))
                _openSection = isOpen ? section : -1;

            float frameHeight = _imgui.GetFrameHeight();
            float itemSpacing = _imgui.GetStyle().ItemSpacing.x;
            float framePadding = _imgui.GetStyle().FramePadding.x;
            float totalWidth = frameHeight * 3 + itemSpacing * 2;
            _imgui.SameLine(
                _imgui.GetContentRegionMax().x - totalWidth - framePadding);

            bool upPressed = false;
            bool downPressed = false;
            {
                ImguiDisabled disabled(i == 0);
                upPressed = _imgui.Button(
                    ICON_CODICON_ARROW_UP, ImVec2(frameHeight, frameHeight));
            }
            _imgui.SameLine(0, itemSpacing);
            {
                ImguiDisabled disabled(i + 1 >= viewOrder.size());
                downPressed = _imgui.Button(
                    ICON_CODICON_ARROW_DOWN, ImVec2(frameHeight, frameHeight));
            }
            _imgui.SameLine(0, itemSpacing);
            if (_imgui.Checkbox("##enabled", &enabled))
                _pendingPreferences->SetBoolean(enabledKey, enabled);
            if (upPressed && i > 0)
            {
                std::vector<std::string> newOrder = viewOrder;
                std::swap(newOrder[i], newOrder[i - 1]);
                _pendingPreferences->SetStringList("views", newOrder);
                if (_openSection == section)
                    _openSection = section - 1;
                else if (_openSection == section - 1)
                    _openSection = section;
            }
            if (downPressed && i + 1 < viewOrder.size())
            {
                std::vector<std::string> newOrder = viewOrder;
                std::swap(newOrder[i], newOrder[i + 1]);
                _pendingPreferences->SetStringList("views", newOrder);
                if (_openSection == section)
                    _openSection = section + 1;
                else if (_openSection == section + 1)
                    _openSection = section;
            }

            if (_openSection == section)
            {
                float indent = _imgui.GetFontSize() * 2.0f;
                _imgui.Indent(indent);
                view->DrawConfiguration(*_pendingPreferences);

                _imgui.PushID("sensors");
                for (Sensor* sensor : view->GetSensors())
                {
                    _imgui.PushID(sensor->GetPrefName().c_str());
                    sensor->DrawConfiguration(*_pendingPreferences);
                    _imgui.PopID();
                }

                _imgui.PopID();
                _imgui.Unindent(indent);
            }
            _imgui.PopID();
        }
    }
    _imgui.EndChild();

    if (_imgui.BeginChild("LowerArea", ImVec2(0, 0), false))
    {
        float okWidth = _imgui.CalcTextSize("OK").x +
                        _imgui.GetStyle().FramePadding.x * 2.0f;
        float cancelWidth = _imgui.CalcTextSize("Cancel").x +
                            _imgui.GetStyle().FramePadding.x * 2.0f;
        float totalWidth =
            okWidth + cancelWidth + _imgui.GetStyle().ItemSpacing.x;
        _imgui.SetCursorPosX(0);
        if (_imgui.Button("Quit!"))
            _app.Quit();
        _imgui.SameLine();
        _imgui.SetCursorPosX(_imgui.GetCursorPosX() +
                             _imgui.GetContentRegionAvail().x - totalWidth);
        if (_imgui.Button("OK"))
        {
            WriteTomlPreferences(*_pendingPreferences);
            longjmp(g_restartJmp, 1);
        }
        _imgui.SameLine();
        if (_imgui.Button("Cancel"))
        {
            _pendingMapPreferences->ClearAll();
            if (open != nullptr)
                *open = false;
        }
    }
    _imgui.EndChild();
}
