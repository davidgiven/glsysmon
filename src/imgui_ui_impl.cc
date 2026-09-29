#include "ui.h"

#include <SDL3/SDL.h>

#include <imgui.h>

#include <memory>
#include <string>
#include <vector>

#include "app.h"
#include "configuration.h"
#include "context.h"
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
        explicit ImGuiUiImpl(const Context& ctx):
            _context(ctx),
            _ownedSensors(std::make_unique<Sensors>(ctx)),
            _sensors(*_ownedSensors),
            _views(ctx, _sensors),
            _app(ctx.app),
            _configurationWindow(_views, _app, _context.imgui)
        {
            for (const std::string& name :
                GlobalPreferencesFetcher::GetViews(ctx.preferences))
            {
                View* view = _views.Get(name);
                if (view == nullptr)
                    continue;
                _activeViews.push_back(view);
            }
        }

        explicit ImGuiUiImpl(const Context& ctx, Sensors& sensors):
            _context(ctx),
            _sensors(sensors),
            _views(ctx, _sensors),
            _app(ctx.app),
            _configurationWindow(_views, _app, _context.imgui)
        {
            for (const std::string& name :
                GlobalPreferencesFetcher::GetViews(ctx.preferences))
            {
                View* view = _views.Get(name);
                if (view == nullptr)
                    continue;
                _activeViews.push_back(view);
            }
        }

        void Draw() override
        {
            ImGuiViewport* viewport = _context.imgui.GetMainViewport();
            _context.imgui.SetNextWindowPos(viewport->Pos);
            _context.imgui.SetNextWindowSize(viewport->Size);
            _context.imgui.PushStyleVar(
                ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            _context.imgui.PushFont(nullptr, _context.imgui.GetStyle().FontSizeBase * 0.75f);
            _context.imgui.Begin("glsysmon",
                nullptr,
                ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
                    ImGuiWindowFlags_NoResize |
                    ImGuiWindowFlags_NoSavedSettings |
                    ImGuiWindowFlags_NoBringToFrontOnFocus);

            for (View* view : _activeViews)
            {
                std::string enabledKey = view->GetPrefName() + ".enabled";
                bool enabled =
                    _context.preferences.GetBoolean(enabledKey).value_or(true);
                if (!enabled)
                    continue;
                view->Draw();
            }

            _context.imgui.PopFont();
            _context.imgui.PopStyleVar();

            _context.imgui.End();

            if (_context.imgui.IsMouseClicked(ImGuiMouseButton_Right))
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
                _context.imgui.SetNextWindowClass(&window_class);
                _context.imgui.SetNextWindowSize(
                    ImVec2(800, 600), ImGuiCond_FirstUseEver);
                _context.imgui.Begin("Configuration",
                    &_viewportOpen,
                    ImGuiWindowFlags_NoTitleBar);

                if (_viewportFocusRequested)
                {
                    ImGuiViewport* vp = _context.imgui.GetWindowViewport();
                    if (vp != nullptr && vp != _context.imgui.GetMainViewport() &&
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
                _context.imgui.End();
            }
        }

    private:
        const Context& _context;
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

std::unique_ptr<Ui> CreateUi(const Context& ctx)
{
    return std::make_unique<ImGuiUiImpl>(ctx);
}

std::unique_ptr<Ui> CreateUi(const Context& ctx, Sensors& sensors)
{
    return std::make_unique<ImGuiUiImpl>(ctx, sensors);
}
