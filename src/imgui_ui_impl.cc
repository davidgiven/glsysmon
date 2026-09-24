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
        explicit ImGuiUiImpl(const Preferences& prefs,
            Timer& timer,
            App& app,
            std::unique_ptr<HostnameSensor> fakeHostnameSensor = nullptr,
            std::unique_ptr<ClockSensor> fakeClockSensor = nullptr,
            std::unique_ptr<CpuSensor> fakeCpuSensor = nullptr,
            std::unique_ptr<TemperatureSensor> fakeTemperatureSensor = nullptr):
            _prefs(prefs),
            _sensors(prefs, timer),
            _views(prefs, _sensors),
            _app(app),
            _configurationWindow(_views, _sensors, _app)
        {
            if (fakeHostnameSensor != nullptr)
                _sensors.SetHostnameSensor(std::move(fakeHostnameSensor));
            if (fakeClockSensor != nullptr)
                _sensors.SetClockSensor(std::move(fakeClockSensor));
            if (fakeCpuSensor != nullptr)
                _sensors.SetCpuSensor(std::move(fakeCpuSensor));
            if (fakeTemperatureSensor != nullptr)
                _sensors.SetTemperatureSensor(std::move(fakeTemperatureSensor));
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
                view->Draw();

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
        Sensors _sensors;
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

std::unique_ptr<Ui> CreateUiWithFakeHostname(const Preferences& prefs,
    Timer& timer,
    std::unique_ptr<HostnameSensor> fakeSensor,
    App& app)
{
    return std::make_unique<ImGuiUiImpl>(
        prefs, timer, app, std::move(fakeSensor), nullptr, nullptr, nullptr);
}

std::unique_ptr<Ui> CreateUiWithFakeClock(const Preferences& prefs,
    Timer& timer,
    std::unique_ptr<ClockSensor> fakeSensor,
    App& app)
{
    return std::make_unique<ImGuiUiImpl>(
        prefs, timer, app, nullptr, std::move(fakeSensor), nullptr, nullptr);
}

std::unique_ptr<Ui> CreateUiWithFakeCpu(const Preferences& prefs,
    Timer& timer,
    std::unique_ptr<CpuSensor> fakeSensor,
    App& app)
{
    return std::make_unique<ImGuiUiImpl>(
        prefs, timer, app, nullptr, nullptr, std::move(fakeSensor), nullptr);
}

std::unique_ptr<Ui> CreateUiWithFakeTemperature(const Preferences& prefs,
    Timer& timer,
    std::unique_ptr<TemperatureSensor> fakeSensor,
    App& app)
{
    return std::make_unique<ImGuiUiImpl>(
        prefs, timer, app, nullptr, nullptr, nullptr, std::move(fakeSensor));
}
