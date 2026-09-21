#include "ui.h"

#include <SDL3/SDL.h>
#include <imgui.h>

#include <memory>
#include <string>
#include <vector>

#include "components.h"
#include "preferences/preferences.h"
#include "sensors/sensors.h"
#include "timer.h"
#include "views/catalogue.h"

namespace
{

    class ImGuiUiImpl : public Ui
    {
    public:
        explicit ImGuiUiImpl(const Preferences& prefs,
            Timer& timer,
            std::unique_ptr<HostnameSensor> fakeHostnameSensor = nullptr,
            std::unique_ptr<ClockSensor> fakeClockSensor = nullptr,
            std::unique_ptr<CpuSensor> fakeCpuSensor = nullptr,
            std::unique_ptr<TemperatureSensor> fakeTemperatureSensor = nullptr):
            _prefs(prefs),
            _sensors(prefs, timer)
        {
            if (fakeHostnameSensor != nullptr)
                _sensors.SetHostnameSensor(std::move(fakeHostnameSensor));
            if (fakeClockSensor != nullptr)
                _sensors.SetClockSensor(std::move(fakeClockSensor));
            if (fakeCpuSensor != nullptr)
                _sensors.SetCpuSensor(std::move(fakeCpuSensor));
            if (fakeTemperatureSensor != nullptr)
                _sensors.SetTemperatureSensor(std::move(fakeTemperatureSensor));
            const auto& catalogue = GetViewCatalogue();
            for (const std::string& name :
                GlobalPreferencesFetcher::GetViews(prefs))
            {
                const auto it = catalogue.find(name);
                if (it == catalogue.end())
                    continue;
                _views.push_back(it->second(prefs, _sensors));
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

            for (auto& view : _views)
                view->Draw();

            ImGui::PopFont();
            ImGui::PopStyleVar();

            ImGuiWindowClass window_class;
            window_class.ViewportFlagsOverrideSet = ImGuiViewportFlags_TopMost;
            ImGui::SetNextWindowClass(&window_class);
            if (ImGui::BeginPopupContextWindow(
                    "item_context_menu")) // opens on right-click of the item
                                          // above
            {
                if (ImGui::MenuItem("Quit"))
                {
                    SDL_Event quitEvent{};
                    quitEvent.type = SDL_EVENT_QUIT;
                    SDL_PushEvent(&quitEvent);
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }

            ImGui::End();

            // if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
            // {
            //     ImVec2 mousePos = ImGui::GetMousePos();
            //     if (mousePos.x >= viewport->Pos.x &&
            //         mousePos.x < viewport->Pos.x + viewport->Size.x &&
            //         mousePos.y >= viewport->Pos.y &&
            //         mousePos.y < viewport->Pos.y + viewport->Size.y)
            //         ImGui::OpenPopup("popup_menu");
            // }

            // if (ImGui::BeginPopup("popup_menu"))
            // {
            //     if (ImGui::MenuItem("Quit"))
            //     {
            //         SDL_Event quitEvent{};
            //         quitEvent.type = SDL_EVENT_QUIT;
            //         SDL_PushEvent(&quitEvent);
            //         ImGui::CloseCurrentPopup();
            //     }
            //     ImGui::Separator();
            //     ImGui::BeginDisabled();
            //     ImGui::MenuItem("Preferences");
            //     ImGui::MenuItem("About");
            //     ImGui::EndDisabled();
            //     ImGui::EndPopup();
            // }
        }

    private:
        const Preferences& _prefs;
        Sensors _sensors;
        std::vector<std::unique_ptr<View>> _views;
    };

} // namespace

std::unique_ptr<Ui> CreateUi(const Preferences& prefs, Timer& timer)
{
    return std::make_unique<ImGuiUiImpl>(prefs, timer);
}

std::unique_ptr<Ui> CreateUiWithFakeHostname(const Preferences& prefs,
    Timer& timer,
    std::unique_ptr<HostnameSensor> fakeSensor)
{
    return std::make_unique<ImGuiUiImpl>(
        prefs, timer, std::move(fakeSensor), nullptr, nullptr, nullptr);
}

std::unique_ptr<Ui> CreateUiWithFakeClock(const Preferences& prefs,
    Timer& timer,
    std::unique_ptr<ClockSensor> fakeSensor)
{
    return std::make_unique<ImGuiUiImpl>(
        prefs, timer, nullptr, std::move(fakeSensor), nullptr, nullptr);
}

std::unique_ptr<Ui> CreateUiWithFakeCpu(const Preferences& prefs,
    Timer& timer,
    std::unique_ptr<CpuSensor> fakeSensor)
{
    return std::make_unique<ImGuiUiImpl>(
        prefs, timer, nullptr, nullptr, std::move(fakeSensor), nullptr);
}

std::unique_ptr<Ui> CreateUiWithFakeTemperature(const Preferences& prefs,
    Timer& timer,
    std::unique_ptr<TemperatureSensor> fakeSensor)
{
    return std::make_unique<ImGuiUiImpl>(
        prefs, timer, nullptr, nullptr, nullptr, std::move(fakeSensor));
}
