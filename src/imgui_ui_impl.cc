#include "ui.h"

#include <SDL3/SDL.h>

#include <imgui.h>

#include <memory>
#include <string>
#include <vector>

#include "app.h"
#include "configuration.h"
#include "preferences/preferences.h"
#include "sensors/sensors.h"
#include "timer.h"
#include "views/views.h"

namespace
{

    class ImGuiUiImpl : public Ui
    {
    public:
        explicit ImGuiUiImpl(const Preferences& prefs, Timer& timer, App& app):
            _prefs(prefs),
            _ownedSensors(std::make_unique<Sensors>(prefs, timer)),
            _sensors(*_ownedSensors),
            _views(prefs, _sensors),
            _app(app),
            _configurationWindow(_views, _app)
        {
            for (const std::string& name :
                GlobalPreferencesFetcher::GetViews(prefs))
            {
                View* view = _views.Get(name);
                if (view == nullptr)
                    continue;
                _activeViews.push_back(view);
            }
        }

        explicit ImGuiUiImpl(
            const Preferences& prefs, Sensors& sensors, App& app):
            _prefs(prefs),
            _sensors(sensors),
            _views(prefs, _sensors),
            _app(app),
            _configurationWindow(_views, _app)
        {
            for (const std::string& name :
                GlobalPreferencesFetcher::GetViews(prefs))
            {
                View* view = _views.Get(name);
                if (view == nullptr)
                    continue;
                _activeViews.push_back(view);
            }
        }

        void Draw() override
        {
            ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->Pos);
            ImGui::SetNextWindowSize(viewport->Size);
            ImGui::PushStyleVar(
                ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            ImGui::PushFont(nullptr, ImGui::GetStyle().FontSizeBase * 0.75f);
            ImGui::Begin("glsysmon",
                nullptr,
                ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
                    ImGuiWindowFlags_NoResize |
                    ImGuiWindowFlags_NoSavedSettings |
                    ImGuiWindowFlags_NoBringToFrontOnFocus);

            for (View* view : _activeViews)
            {
                std::string enabledKey = view->GetPrefName() + ".enabled";
                bool enabled = _prefs.GetBoolean(enabledKey).value_or(true);
                if (!enabled)
                    continue;
                view->Draw();
            }

            ImGui::PopFont();
            ImGui::PopStyleVar();

            ImGui::End();

            if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
            {
                if (!_viewportOpen)
                    _viewportOpen = true;
                _viewportFocusRequested = true;
            }

            if (_viewportOpen)
            {
                ImGuiWindowClass window_class;
                window_class.ViewportFlagsOverrideSet =
                    ImGuiViewportFlags_NoAutoMerge;
                window_class.ViewportFlagsOverrideClear =
                    ImGuiViewportFlags_NoDecoration;
                ImGui::SetNextWindowClass(&window_class);
                ImGui::SetNextWindowSize(
                    ImVec2(600, 500), ImGuiCond_FirstUseEver);
                ImGui::Begin("Configuration",
                    &_viewportOpen,
                    ImGuiWindowFlags_NoTitleBar);

                if (_viewportFocusRequested)
                {
                    ImGuiViewport* vp = ImGui::GetWindowViewport();
                    if (vp != nullptr && vp != ImGui::GetMainViewport() &&
                        vp->PlatformHandle != nullptr)
                    {
                        SDL_Window* sdlWin = SDL_GetWindowFromID(
                            (SDL_WindowID)(uintptr_t)vp->PlatformHandle);
                        if (sdlWin != nullptr)
                        {
                            SDL_RaiseWindow(sdlWin);
                            _viewportFocusRequested = false;
                        }
                    }
                }
                _configurationWindow.Draw(&_viewportOpen);
                ImGui::End();
            }
        }

    private:
        const Preferences& _prefs;
        std::unique_ptr<Sensors> _ownedSensors;
        Sensors& _sensors;
        Views _views;
        App& _app;
        std::vector<View*> _activeViews;
        ConfigurationWindow _configurationWindow;
        bool _viewportOpen = false;
        bool _viewportFocusRequested = false;
    };

} // namespace

std::unique_ptr<Ui> CreateUi(const Preferences& prefs, Timer& timer, App& app)
{
    return std::make_unique<ImGuiUiImpl>(prefs, timer, app);
}

std::unique_ptr<Ui> CreateUi(
    const Preferences& prefs, Sensors& sensors, App& app)
{
    return std::make_unique<ImGuiUiImpl>(prefs, sensors, app);
}
