#include "app.h"

#include <SDL3/SDL.h>
#include <fruit/fruit.h>

#include <utility>

#include "components.h"
#include "imgui_frame_renderer.h"

namespace
{

    class ImGuiAppImpl : public App
    {
    public:
        ImGuiAppImpl(
            DockFactory dockFactory, Ui* ui, ImGuiFrameRenderer* frameRenderer):
            _dockFactory(std::move(dockFactory)),
            _ui(ui),
            _frameRenderer(frameRenderer)
        {
        }

        using Inject = ImGuiAppImpl(DockFactory, Ui*, ImGuiFrameRenderer*);

        int Setup() override
        {
            if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_GAMEPAD))
            {
                SDL_Log("SDL_Init failed: %s", SDL_GetError());
                return 1;
            }

            _dock = _dockFactory();
            _window = _dock->CreateWindow();
            if (_window == nullptr)
            {
                SDL_Log("Dock::CreateWindow failed: %s", SDL_GetError());
                SDL_Quit();
                return 1;
            }
            _backend = SDL_GetCurrentVideoDriver();

            _device =
                SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV, false, nullptr);
            if (_device == nullptr)
            {
                SDL_Log("SDL_CreateGPUDevice failed: %s", SDL_GetError());
                SDL_DestroyWindow(_window);
                _window = nullptr;
                SDL_Quit();
                return 1;
            }
            if (!SDL_ClaimWindowForGPUDevice(_device, _window))
            {
                SDL_Log(
                    "SDL_ClaimWindowForGPUDevice failed: %s", SDL_GetError());
                SDL_DestroyGPUDevice(_device);
                _device = nullptr;
                SDL_DestroyWindow(_window);
                _window = nullptr;
                SDL_Quit();
                return 1;
            }
            SDL_SetGPUSwapchainParameters(_device,
                _window,
                SDL_GPU_SWAPCHAINCOMPOSITION_SDR,
                SDL_GPU_PRESENTMODE_VSYNC);

            if (!_frameRenderer->Init(_device,
                    _window,
                    SDL_GetGPUSwapchainTextureFormat(_device, _window)))
            {
                SDL_Log("ImGuiFrameRenderer::Init failed");
                SDL_ReleaseWindowFromGPUDevice(_device, _window);
                SDL_DestroyGPUDevice(_device);
                _device = nullptr;
                SDL_DestroyWindow(_window);
                _window = nullptr;
                SDL_Quit();
                return 1;
            }
            return 0;
        }

        bool Tick() override
        {
            bool running = true;
            SDL_Event event;
            while (SDL_PollEvent(&event))
            {
                _frameRenderer->ProcessEvent(&event);
                if (event.type == SDL_EVENT_QUIT)
                    running = false;
                if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED &&
                    event.window.windowID == SDL_GetWindowID(_window))
                    running = false;
            }
            _dock->PollWindow(_window);

            _frameRenderer->BeginFrame();
            _ui->Draw(_window, _backend);

            SDL_GPUCommandBuffer* command_buffer =
                SDL_AcquireGPUCommandBuffer(_device);
            SDL_GPUTexture* swapchain_texture = nullptr;
            SDL_WaitAndAcquireGPUSwapchainTexture(
                command_buffer, _window, &swapchain_texture, nullptr, nullptr);
            _frameRenderer->Render(command_buffer, swapchain_texture);
            SDL_SubmitGPUCommandBuffer(command_buffer);
            return running;
        }

        void Shutdown() override
        {
            SDL_WaitForGPUIdle(_device);
            _frameRenderer->Shutdown();
            SDL_ReleaseWindowFromGPUDevice(_device, _window);
            SDL_DestroyGPUDevice(_device);
            SDL_DestroyWindow(_window);
            SDL_Quit();
            _device = nullptr;
            _window = nullptr;
        }

    private:
        DockFactory _dockFactory;
        Ui* _ui;
        ImGuiFrameRenderer* _frameRenderer;
        std::unique_ptr<Dock> _dock;
        SDL_Window* _window = nullptr;
        SDL_GPUDevice* _device = nullptr;
        const char* _backend = nullptr;
    };

} // namespace

fruit::Component<App> GetAppComponent(CliArgs* args)
{
    return fruit::createComponent()
        .install(GetDockComponent)
        .install(GetUiComponent)
        .install(GetImGuiFrameRendererComponent)
        .bindInstance(*args)
        .bind<App, ImGuiAppImpl>();
}
