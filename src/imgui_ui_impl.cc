#include "ui.h"

#include <SDL3/SDL.h>
#include <imgui.h>

#include <memory>
#include <string>
#include <vector>

#include "components.h"
#include "preferences.h"
#include "sensors/sensors.h"
#include "views/catalogue.h"

namespace
{

    class ImGuiUiImpl : public Ui
    {
    public:
        explicit ImGuiUiImpl(const Preferences& prefs,
            std::unique_ptr<HostnameSensor> fakeHostnameSensor = nullptr,
            std::unique_ptr<ClockSensor> fakeClockSensor = nullptr):
            _prefs(&prefs)
        {
            if (fakeHostnameSensor != nullptr)
                _sensors.SetHostnameSensor(std::move(fakeHostnameSensor));
            if (fakeClockSensor != nullptr)
                _sensors.SetClockSensor(std::move(fakeClockSensor));
            const auto& catalogue = GetViewCatalogue();
            for (const std::string& name :
                GlobalPreferencesFetcher::GetViews(prefs))
            {
                const auto it = catalogue.find(name);
                if (it == catalogue.end())
                    continue;
                _views.push_back(it->second(_sensors));
            }
        }

        void Draw(SDL_Window* window, const char* backend) override
        {
            ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->Pos);
            ImGui::SetNextWindowSize(viewport->Size);
            ImGui::Begin("glsysmon",
                nullptr,
                ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
                    ImGuiWindowFlags_NoResize |
                    ImGuiWindowFlags_NoSavedSettings |
                    ImGuiWindowFlags_NoBringToFrontOnFocus);

            for (auto& view : _views)
                view->Tick();
            ImGui::Separator();
            int width, height;
            SDL_GetWindowSize(window, &width, &height);
            const float scale =
                SDL_GetDisplayContentScale(SDL_GetDisplayForWindow(window));
            ImGui::Text("backend: %s", backend);
            ImGui::Text("window: %d x %d", width, height);
            ImGui::Text("display scale: %.2f", scale);
            ImGui::Text("%.1f FPS (%.3f ms/frame)",
                ImGui::GetIO().Framerate,
                1000.0f / ImGui::GetIO().Framerate);

            ImGui::End();
        }

    private:
        const Preferences* _prefs;
        Sensors _sensors;
        std::vector<std::unique_ptr<View>> _views;
    };

} // namespace

std::unique_ptr<Ui> CreateUi(const Preferences& prefs)
{
    return std::make_unique<ImGuiUiImpl>(prefs);
}

std::unique_ptr<Ui> CreateUiWithFakeHostname(
    const Preferences& prefs, std::unique_ptr<HostnameSensor> fakeSensor)
{
    return std::make_unique<ImGuiUiImpl>(prefs, std::move(fakeSensor), nullptr);
}

std::unique_ptr<Ui> CreateUiWithFakeClock(
    const Preferences& prefs, std::unique_ptr<ClockSensor> fakeSensor)
{
    return std::make_unique<ImGuiUiImpl>(prefs, nullptr, std::move(fakeSensor));
}
