#include "app.h"

#include <SDL3/SDL.h>

#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

#include "components.h"
#include "display/imgui_frame_renderer.h"
#include "preferences/preferences.h"

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
        ImGuiAppImpl(DockFactory dockFactory,
            std::unique_ptr<Ui> ui,
            std::unique_ptr<ImGuiFrameRenderer> frameRenderer,
            std::unique_ptr<Preferences> prefs):
            _prefs(std::move(prefs)),
            _dockFactory(std::move(dockFactory)),
            _ui(std::move(ui)),
            _frameRenderer(std::move(frameRenderer))
        {
        }

        ~ImGuiAppImpl() override
        {
            Shutdown();
        }

        void Setup() override
        {
            const int updateFps = GlobalPreferencesFetcher::GetFps(*_prefs);
            const int redrawFps =
                GlobalPreferencesFetcher::GetRedrawFps(*_prefs);
            if (updateFps <= 0)
                throw AppError("fps must be > 0");
            if (redrawFps <= 0)
                throw AppError("redraw_fps must be > 0");
            _updateNs = 1000000000ULL / static_cast<Uint64>(updateFps);
            _drawNs = 1000000000ULL / static_cast<Uint64>(redrawFps);

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
            const Uint64 frame_start = SDL_GetTicksNS();
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

            const Uint64 now = SDL_GetTicksNS();
            if (_lastUpdateNs == 0)
                _lastUpdateNs = now;
            if (now - _lastUpdateNs >= _updateNs)
            {
                _ui->Tick();
                _lastUpdateNs = now;
            }

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
            const Uint64 elapsed = SDL_GetTicksNS() - frame_start;
            if (elapsed < _drawNs)
                SDL_DelayNS(_drawNs - elapsed);
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
        std::unique_ptr<Preferences> _prefs;
        DockFactory _dockFactory;
        std::unique_ptr<Ui> _ui;
        std::unique_ptr<ImGuiFrameRenderer> _frameRenderer;
        std::unique_ptr<Dock> _dock;
        std::unique_ptr<SdlSession> _sdl;
        std::unique_ptr<DockWindow> _window;
        std::unique_ptr<GpuDevice> _device;
        const char* _backend = nullptr;
        bool _rendererInited = false;
        Uint64 _updateNs = 0;
        Uint64 _drawNs = 0;
        Uint64 _lastUpdateNs = 0;
    };

} // namespace

std::unique_ptr<App> CreateApp(const CliArgs& args)
{
    auto prefs = CreatePreferences(args);
    auto dockFactory = CreateDockFactory(*prefs);
    auto ui = CreateUi(*prefs);
    auto renderer = CreateImGuiFrameRenderer();
    return std::make_unique<ImGuiAppImpl>(std::move(dockFactory),
        std::move(ui),
        std::move(renderer),
        std::move(prefs));
}
