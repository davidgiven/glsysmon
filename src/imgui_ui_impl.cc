#include "ui.h"

#include <SDL3/SDL.h>
#include <fruit/fruit.h>
#include <imgui.h>

#include <string>
#include <vector>

#include "components.h"
#include "preferences.h"
#include "views/catalogue.h"

namespace
{

    class ImGuiUiImpl : public Ui
    {
    public:
        using Inject = ImGuiUiImpl(Preferences*);

        explicit ImGuiUiImpl(Preferences* prefs): _prefs(prefs)
        {
            for (const std::string& name :
                GlobalPreferencesFetcher::GetViews(*prefs))
            {
                const auto& catalogue = GetViewCatalogue();
                const auto it = catalogue.find(name);
                if (it == catalogue.end())
                    continue;
                _views.push_back(fruit::Injector<View>(it->second));
            }
        }

        void Draw(SDL_Window* window, const char* backend) override
        {
            ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->Pos);
            ImGui::SetNextWindowSize(viewport->Size);
            ImGui::Begin("glrellm",
                nullptr,
                ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
                    ImGuiWindowFlags_NoResize |
                    ImGuiWindowFlags_NoSavedSettings |
                    ImGuiWindowFlags_NoBringToFrontOnFocus);

            for (fruit::Injector<View>& injector : _views)
                injector.get<View*>()->Tick();
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
        Preferences* _prefs;
        std::vector<fruit::Injector<View>> _views;
    };

} // namespace

fruit::Component<fruit::Required<CliArgs>, Ui> GetUiComponent()
{
    return fruit::createComponent()
        .install(GetPreferencesComponent)
        .bind<Ui, ImGuiUiImpl>();
}
