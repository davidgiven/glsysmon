#include "ui.h"

#include <SDL3/SDL.h>
#include <imgui.h>

void UiDraw(SDL_Window *window, const char *backend)
{
    ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::Begin("glrellm", nullptr,
                 ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
                     ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings |
                     ImGuiWindowFlags_NoBringToFrontOnFocus);

    int width, height;
    SDL_GetWindowSize(window, &width, &height);
    const float scale = SDL_GetDisplayContentScale(SDL_GetDisplayForWindow(window));
    ImGui::Text("glrellm");
    ImGui::Separator();
    ImGui::Text("backend: %s", backend);
    ImGui::Text("window: %d x %d", width, height);
    ImGui::Text("display scale: %.2f", scale);
    ImGui::Text("%.1f FPS (%.3f ms/frame)", ImGui::GetIO().Framerate,
                1000.0f / ImGui::GetIO().Framerate);

    ImGui::End();
}
