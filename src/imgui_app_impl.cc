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
        : _dockFactory(std::move(dockFactory)), _ui(ui)
    {
    }

    using Inject = ImGuiAppImpl(DockFactory, Ui *);

    int Setup() override
    {
        if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_GAMEPAD)) {
            SDL_Log("SDL_Init failed: %s", SDL_GetError());
            return 1;
        }

        _dock = _dockFactory();
        _window = _dock->CreateWindow();
        if (_window == nullptr) {
            SDL_Log("Dock::CreateWindow failed: %s", SDL_GetError());
            SDL_Quit();
            return 1;
        }
        _backend = SDL_GetCurrentVideoDriver();

        _device = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV, false, nullptr);
        if (_device == nullptr) {
            SDL_Log("SDL_CreateGPUDevice failed: %s", SDL_GetError());
            SDL_DestroyWindow(_window);
            _window = nullptr;
            SDL_Quit();
            return 1;
        }
        if (!SDL_ClaimWindowForGPUDevice(_device, _window)) {
            SDL_Log("SDL_ClaimWindowForGPUDevice failed: %s", SDL_GetError());
            SDL_DestroyGPUDevice(_device);
            _device = nullptr;
            SDL_DestroyWindow(_window);
            _window = nullptr;
            SDL_Quit();
            return 1;
        }
        SDL_SetGPUSwapchainParameters(_device, _window, SDL_GPU_SWAPCHAINCOMPOSITION_SDR,
                                      SDL_GPU_PRESENTMODE_VSYNC);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.IniFilename = nullptr;  // do not persist window layout to imgui.ini
        ImGui::StyleColorsDark();

        const float ui_scale = SDL_GetDisplayContentScale(SDL_GetDisplayForWindow(_window));
        ImGuiStyle &style = ImGui::GetStyle();
        style.ScaleAllSizes(ui_scale);
        style.FontScaleDpi = ui_scale;

        ImGui_ImplSDL3_InitForSDLGPU(_window);
        ImGui_ImplSDLGPU3_InitInfo init_info = {};
        init_info.Device = _device;
        init_info.ColorTargetFormat = SDL_GetGPUSwapchainTextureFormat(_device, _window);
        init_info.MSAASamples = SDL_GPU_SAMPLECOUNT_1;
        ImGui_ImplSDLGPU3_Init(&init_info);
        return 0;
    }

    bool Tick() override
    {
        bool running = true;
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL3_ProcessEvent(&event);
            if (event.type == SDL_EVENT_QUIT)
                running = false;
            if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED &&
                event.window.windowID == SDL_GetWindowID(_window))
                running = false;
        }
        _dock->PollWindow(_window);

        ImGui_ImplSDLGPU3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();
        _ui->Draw(_window, _backend);
        ImGui::Render();

        ImDrawData *draw_data = ImGui::GetDrawData();
        const bool is_minimized =
            draw_data->DisplaySize.x <= 0.0f || draw_data->DisplaySize.y <= 0.0f;

        SDL_GPUCommandBuffer *command_buffer = SDL_AcquireGPUCommandBuffer(_device);
        SDL_GPUTexture *swapchain_texture = nullptr;
        SDL_WaitAndAcquireGPUSwapchainTexture(command_buffer, _window, &swapchain_texture,
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
        return running;
    }

    void Shutdown() override
    {
        SDL_WaitForGPUIdle(_device);
        ImGui_ImplSDL3_Shutdown();
        ImGui_ImplSDLGPU3_Shutdown();
        ImGui::DestroyContext();
        SDL_ReleaseWindowFromGPUDevice(_device, _window);
        SDL_DestroyGPUDevice(_device);
        SDL_DestroyWindow(_window);
        SDL_Quit();
        _device = nullptr;
        _window = nullptr;
    }

private:
    DockFactory _dockFactory;
    Ui *_ui;
    std::unique_ptr<Dock> _dock;
    SDL_Window *_window = nullptr;
    SDL_GPUDevice *_device = nullptr;
    const char *_backend = nullptr;
};

}  // namespace

fruit::Component<App> GetAppComponent(CliArgs *args)
{
    return fruit::createComponent()
        .install(GetDockComponent)
        .install(GetUiComponent)
        .bindInstance(*args)
        .bind<App, ImGuiAppImpl>();
}
