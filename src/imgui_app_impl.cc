#include "app.h"

#include <SDL3/SDL.h>
#include <fruit/fruit.h>

#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

#include "components.h"
#include "imgui_frame_renderer.h"

namespace
{

    class AppError : public std::runtime_error
    {
    public:
        using std::runtime_error::runtime_error;
    };

    // SDL_Init / SDL_Quit scope guard.
    class SdlSession
    {
    public:
        SdlSession()
        {
            if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_GAMEPAD))
                throw AppError(
                    "SDL_Init failed: " + std::string(SDL_GetError()));
        }

        ~SdlSession()
        {
            SDL_Quit();
        }

        SdlSession(const SdlSession&) = delete;
        SdlSession& operator=(const SdlSession&) = delete;
    };

    // SDL_Window scope guard; the window comes from the dock backend.
    class DockWindow
    {
    public:
        explicit DockWindow(SDL_Window* window): _window(window)
        {
            if (_window == nullptr)
                throw AppError("Dock::CreateWindow failed: " +
                               std::string(SDL_GetError()));
        }

        ~DockWindow()
        {
            if (_window != nullptr)
                SDL_DestroyWindow(_window);
        }

        DockWindow(const DockWindow&) = delete;
        DockWindow& operator=(const DockWindow&) = delete;

        SDL_Window* get() const
        {
            return _window;
        }

    private:
        SDL_Window* _window;
    };

    // SDL_GPU device scope guard; releases a claimed window before destroying
    // the device, so it must be destroyed before the window.
    class GpuDevice
    {
    public:
        GpuDevice():
            _device(
                SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV, false, nullptr))
        {
            if (_device == nullptr)
                throw AppError("SDL_CreateGPUDevice failed: " +
                               std::string(SDL_GetError()));
        }

        ~GpuDevice()
        {
            if (_device == nullptr)
                return;
            if (_window != nullptr)
                SDL_ReleaseWindowFromGPUDevice(_device, _window);
            SDL_DestroyGPUDevice(_device);
        }

        GpuDevice(const GpuDevice&) = delete;
        GpuDevice& operator=(const GpuDevice&) = delete;

        SDL_GPUDevice* get() const
        {
            return _device;
        }

        void ClaimWindow(SDL_Window* window)
        {
            if (!SDL_ClaimWindowForGPUDevice(_device, window))
                throw AppError("SDL_ClaimWindowForGPUDevice failed: " +
                               std::string(SDL_GetError()));
            _window = window;
        }

    private:
        SDL_GPUDevice* _device;
        SDL_Window* _window = nullptr;
    };

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

        ~ImGuiAppImpl() override
        {
            Shutdown();
        }

        void Setup() override
        {
            _sdl = std::make_unique<SdlSession>();
            _dock = _dockFactory();
            _window = std::make_unique<DockWindow>(_dock->CreateWindow());
            _backend = SDL_GetCurrentVideoDriver();

            _device = std::make_unique<GpuDevice>();
            _device->ClaimWindow(_window->get());
            SDL_SetGPUSwapchainParameters(_device->get(),
                _window->get(),
                SDL_GPU_SWAPCHAINCOMPOSITION_SDR,
                SDL_GPU_PRESENTMODE_VSYNC);

            if (!_frameRenderer->Init(_device->get(),
                    _window->get(),
                    SDL_GetGPUSwapchainTextureFormat(
                        _device->get(), _window->get())))
                throw AppError("ImGuiFrameRenderer::Init failed");
            _rendererInited = true;
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
                    event.window.windowID == SDL_GetWindowID(_window->get()))
                    running = false;
            }
            _dock->PollWindow(_window->get());

            _frameRenderer->BeginFrame();
            _ui->Draw(_window->get(), _backend);

            SDL_GPUCommandBuffer* command_buffer =
                SDL_AcquireGPUCommandBuffer(_device->get());
            SDL_GPUTexture* swapchain_texture = nullptr;
            SDL_WaitAndAcquireGPUSwapchainTexture(command_buffer,
                _window->get(),
                &swapchain_texture,
                nullptr,
                nullptr);
            _frameRenderer->Render(command_buffer, swapchain_texture);
            SDL_SubmitGPUCommandBuffer(command_buffer);
            return running;
        }

        void Shutdown() override
        {
            if (_device != nullptr && _rendererInited)
            {
                SDL_WaitForGPUIdle(_device->get());
                _frameRenderer->Shutdown();
                _rendererInited = false;
            }
            _device.reset();
            _window.reset();
            _sdl.reset();
            _dock.reset();
        }

    private:
        DockFactory _dockFactory;
        Ui* _ui;
        ImGuiFrameRenderer* _frameRenderer;
        std::unique_ptr<Dock> _dock;
        std::unique_ptr<SdlSession> _sdl;
        std::unique_ptr<DockWindow> _window;
        std::unique_ptr<GpuDevice> _device;
        const char* _backend = nullptr;
        bool _rendererInited = false;
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
