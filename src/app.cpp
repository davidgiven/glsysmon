#include "app.h"

#include <SDL3/SDL.h>
#include <fruit/fruit.h>
#include <imgui.h>

#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlgpu3.h>

#include <utility>

#include "components.h"

namespace {

class ImGuiAppImpl : public App {
public:
    ImGuiAppImpl(DockFactory dockFactory, Ui *ui)
        : dockFactory_(std::move(dockFactory)), ui_(ui)
    {
    }

    using Inject = ImGuiAppImpl(DockFactory, Ui *);

    int Run(const DockConfig &cfg) override
    {
        if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_GAMEPAD)) {
            SDL_Log("SDL_Init failed: %s", SDL_GetError());
            return 1;
        }

        std::unique_ptr<Dock> dock = dockFactory_(cfg);
        SDL_Window *window = dock->CreateWindow();
        if (window == nullptr) {
            SDL_Log("Dock::CreateWindow failed: %s", SDL_GetError());
            SDL_Quit();
            return 1;
        }
        const char *backend = SDL_GetCurrentVideoDriver();

        SDL_GPUDevice *device = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV, false, nullptr);
        if (device == nullptr) {
            SDL_Log("SDL_CreateGPUDevice failed: %s", SDL_GetError());
            SDL_DestroyWindow(window);
            SDL_Quit();
            return 1;
        }
        if (!SDL_ClaimWindowForGPUDevice(device, window)) {
            SDL_Log("SDL_ClaimWindowForGPUDevice failed: %s", SDL_GetError());
            SDL_DestroyGPUDevice(device);
            SDL_DestroyWindow(window);
            SDL_Quit();
            return 1;
        }
        SDL_SetGPUSwapchainParameters(device, window, SDL_GPU_SWAPCHAINCOMPOSITION_SDR,
                                      SDL_GPU_PRESENTMODE_VSYNC);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.IniFilename = nullptr;  // do not persist window layout to imgui.ini
        ImGui::StyleColorsDark();

        const float ui_scale = SDL_GetDisplayContentScale(SDL_GetDisplayForWindow(window));
        ImGuiStyle &style = ImGui::GetStyle();
        style.ScaleAllSizes(ui_scale);
        style.FontScaleDpi = ui_scale;

        ImGui_ImplSDL3_InitForSDLGPU(window);
        ImGui_ImplSDLGPU3_InitInfo init_info = {};
        init_info.Device = device;
        init_info.ColorTargetFormat = SDL_GetGPUSwapchainTextureFormat(device, window);
        init_info.MSAASamples = SDL_GPU_SAMPLECOUNT_1;
        ImGui_ImplSDLGPU3_Init(&init_info);

        bool done = false;
        while (!done) {
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                ImGui_ImplSDL3_ProcessEvent(&event);
                if (event.type == SDL_EVENT_QUIT)
                    done = true;
                if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED &&
                    event.window.windowID == SDL_GetWindowID(window))
                    done = true;
            }
            dock->PollWindow(window);

            ImGui_ImplSDLGPU3_NewFrame();
            ImGui_ImplSDL3_NewFrame();
            ImGui::NewFrame();
            ui_->Draw(window, backend);
            ImGui::Render();

            ImDrawData *draw_data = ImGui::GetDrawData();
            const bool is_minimized =
                draw_data->DisplaySize.x <= 0.0f || draw_data->DisplaySize.y <= 0.0f;

            SDL_GPUCommandBuffer *command_buffer = SDL_AcquireGPUCommandBuffer(device);
            SDL_GPUTexture *swapchain_texture = nullptr;
            SDL_WaitAndAcquireGPUSwapchainTexture(command_buffer, window, &swapchain_texture,
                                                  nullptr, nullptr);
            if (swapchain_texture != nullptr && !is_minimized) {
                // Required before a render pass that draws ImGui.
                ImGui_ImplSDLGPU3_PrepareDrawData(draw_data, command_buffer);

                SDL_GPUColorTargetInfo target_info = {};
                target_info.texture = swapchain_texture;
                target_info.load_op = SDL_GPU_LOADOP_CLEAR;
                target_info.store_op = SDL_GPU_STOREOP_STORE;
                target_info.clear_color = {0.08f, 0.08f, 0.10f, 1.0f};
                target_info.mip_level = 0;
                target_info.layer_or_depth_plane = 0;
                target_info.cycle = false;
                SDL_GPURenderPass *render_pass =
                    SDL_BeginGPURenderPass(command_buffer, &target_info, 1, nullptr);
                ImGui_ImplSDLGPU3_RenderDrawData(draw_data, command_buffer, render_pass);
                SDL_EndGPURenderPass(render_pass);
            }
            SDL_SubmitGPUCommandBuffer(command_buffer);
        }

        SDL_WaitForGPUIdle(device);
        ImGui_ImplSDL3_Shutdown();
        ImGui_ImplSDLGPU3_Shutdown();
        ImGui::DestroyContext();
        SDL_ReleaseWindowFromGPUDevice(device, window);
        SDL_DestroyGPUDevice(device);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 0;
    }

private:
    DockFactory dockFactory_;
    Ui *ui_;
};

}  // namespace

fruit::Component<App> GetAppComponent()
{
    return fruit::createComponent()
        .install(GetDockComponent)
        .install(GetUiComponent)
        .bind<App, ImGuiAppImpl>();
}
