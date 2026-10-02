#include "ui.h"

#include <SDL3/SDL.h>

#include <imgui.h>

#include <memory>
#include <string>
#include <vector>

#include "app.h"
#include "configuration.h"
#include "imguiif.h"
#include "preferences/preferences.h"
#include "sensors/sensors.h"
#include "timer.h"
#include "views/views.h"

namespace
{

    class ImGuiUiImpl : public Ui
    {
    public:
        explicit ImGuiUiImpl(App& app, Sensors& sensors):
            _app(app),
            _sensors(sensors),
            _views(app, _sensors),
            _configurationWindow(_views, _app, _app.GetImGui())
        {
            for (const std::string& name :
                GlobalPreferencesFetcher::GetViews(app.GetPreferences()))
            {
                View* view = _views.Get(name);
                if (view == nullptr)
                    continue;
                _activeViews.push_back(view);
            }
        }

        void Draw() override
        {
            ImGuiViewport* viewport = _app.GetImGui().GetMainViewport();
            _app.GetImGui().SetNextWindowPos(viewport->Pos);
            _app.GetImGui().SetNextWindowSize(viewport->Size);
            _app.GetImGui().PushStyleVar(
                ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            _app.GetImGui().Begin("glsysmon",
                nullptr,
                ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
                    ImGuiWindowFlags_NoResize |
                    ImGuiWindowFlags_NoSavedSettings |
                    ImGuiWindowFlags_NoBringToFrontOnFocus);

            for (View* view : _activeViews)
            {
                std::string enabledKey = view->GetPrefName() + ".enabled";
                bool enabled =
                    _app.GetPreferences().GetBoolean(enabledKey).value_or(true);
                if (!enabled)
                    continue;
                view->Draw();
            }

            _app.GetImGui().PopStyleVar();

            _app.GetImGui().End();

            if (_app.GetImGui().IsMouseClicked(ImGuiMouseButton_Right))
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
                _app.GetImGui().SetNextWindowClass(&window_class);
                _app.GetImGui().SetNextWindowSize(
                    ImVec2(800, 600), ImGuiCond_FirstUseEver);
                _app.GetImGui().Begin("Configuration",
                    &_viewportOpen,
                    ImGuiWindowFlags_NoTitleBar);

                if (_viewportFocusRequested)
                {
                    ImGuiViewport* vp = _app.GetImGui().GetWindowViewport();
                    if (vp != nullptr &&
                        vp != _app.GetImGui().GetMainViewport() &&
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
                _app.GetImGui().End();
            }
        }

    private:
        App& _app;
        Sensors& _sensors;
        Views _views;
        std::vector<View*> _activeViews;
        ConfigurationWindow _configurationWindow;
        bool _viewportOpen = false;
        bool _viewportFocusRequested = false;
    };

} // namespace

std::unique_ptr<Ui> CreateUi(App& app, Sensors& sensors)
{
    return std::make_unique<ImGuiUiImpl>(app, sensors);
}
