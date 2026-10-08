#include "app.h"

#include <SDL3/SDL.h>

#include <imgui.h>

#include <algorithm>
#include <limits>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <utility>

#include "display/dock.h"
#include "display/imgui_frame_renderer.h"
#include "imguiif.h"
#include "preferences/preferences.h"
#include "sensors/sensors.h"
#include "timer.h"
#include "ui.h"

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
            std::unique_ptr<ImGuiFrameRenderer> frameRenderer,
            std::shared_ptr<Preferences> prefs,
            std::unique_ptr<Timer> timer,
            std::unique_ptr<ImGuiIf> imgui):
            _prefs(std::move(prefs)),
            _dockFactory(std::move(dockFactory)),
            _frameRenderer(std::move(frameRenderer)),
            _timer(std::move(timer)),
            _imgui(std::move(imgui))
        {
        }

        ~ImGuiAppImpl() override
        {
            Shutdown();
        }

        std::shared_ptr<Preferences> GetSharedPreferences() override
        {
            return _prefs;
        }

        Preferences& GetPreferences() override
        {
            return *_prefs;
        }

        const Preferences& GetPreferences() const override
        {
            return *_prefs;
        }

        Timer& GetTimer() override
        {
            return *_timer;
        }

        ImGuiIf& GetImGui() override
        {
            return *_imgui;
        }

        const Timer& GetTimer() const override
        {
            return *_timer;
        }

        const ImGuiIf& GetImGui() const override
        {
            return *_imgui;
        }

        void SetUi(std::unique_ptr<Ui> ui)
        {
            _ui = std::move(ui);
        }

        void SetSensors(std::unique_ptr<Sensors> sensors)
        {
            _sensors = std::move(sensors);
        }

        void Setup() override
        {
            const double fps = GlobalPreferencesFetcher::GetFps(*_prefs);
            if (fps <= 0.0)
                throw AppError("fps must be > 0");
            _drawNs = static_cast<Uint64>(1'000'000'000.0 / fps);

            _sdl = std::make_unique<SdlSession>();
            _dock = _dockFactory();
            _window = std::make_unique<DockWindow>(_dock->CreateWindow());

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

        void EnsureConfigWindow()
        {
            if (_configWindow != nullptr)
                return;
            SDL_Window* win = SDL_CreateWindow("glsysmon Configuration",
                800,
                600,
                SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALWAYS_ON_TOP);
            if (win == nullptr)
                throw AppError("Config window creation failed: " +
                               std::string(SDL_GetError()));
            if (!SDL_ClaimWindowForGPUDevice(_device->get(), win))
            {
                SDL_DestroyWindow(win);
                throw AppError("SDL_ClaimWindowForGPUDevice (config) failed: " +
                               std::string(SDL_GetError()));
            }
            SDL_SetGPUSwapchainParameters(_device->get(),
                win,
                SDL_GPU_SWAPCHAINCOMPOSITION_SDR,
                SDL_GPU_PRESENTMODE_VSYNC);
            auto renderer = CreateImGuiFrameRenderer(*_prefs);
            if (!renderer->Init(_device->get(),
                    win,
                    SDL_GetGPUSwapchainTextureFormat(_device->get(), win)))
            {
                SDL_ReleaseWindowFromGPUDevice(_device->get(), win);
                SDL_DestroyWindow(win);
                throw AppError("ImGuiFrameRenderer::Init (config) failed");
            }
            _configWindow = win;
            _configWindowID = SDL_GetWindowID(win);
            _configRenderer = std::move(renderer);
            _configRendererInited = true;
            SDL_RaiseWindow(win);
            SDL_ShowWindow(win);
        }

        void DestroyConfigWindow()
        {
            if (_configRenderer && _configRendererInited)
            {
                if (_device && _device->get())
                    SDL_WaitForGPUIdle(_device->get());
                _configRenderer->Shutdown();
                _configRendererInited = false;
            }
            _configRenderer.reset();
            if (_configWindow != nullptr)
            {
                if (_device && _device->get())
                    SDL_ReleaseWindowFromGPUDevice(
                        _device->get(), _configWindow);
                SDL_DestroyWindow(_configWindow);
                _configWindow = nullptr;
                _configWindowID = 0;
            }
            if (_ui)
                _ui->SetConfigOpen(false);
        }

        void MainLoop() override
        {
            Redraw();
            _lastRedrawNs = SDL_GetTicksNS();
            bool pendingRedraw = false;
            while (_running)
            {
                Sint32 timeoutMs =
                    ComputeTimeoutMs(SDL_GetTicksNS(), pendingRedraw);

                SDL_Event event;
                bool hasEvent = (timeoutMs < 0)
                                    ? SDL_WaitEvent(&event)
                                    : SDL_WaitEventTimeout(&event, timeoutMs);
                if (hasEvent)
                {
                    HandleEvent(event);
                    pendingRedraw = true;
                }

                _dock->PollWindow(_window->get());

                Uint64 now = SDL_GetTicksNS();
                if (_timer->Tick(now) > 0)
                    pendingRedraw = true;

                if (_ui && _ui->IsConfigOpen() && _configWindow == nullptr)
                    pendingRedraw = true;
                if (_ui && !_ui->IsConfigOpen() && _configWindow != nullptr)
                    pendingRedraw = true;

                if (pendingRedraw)
                    TryRedraw(now, pendingRedraw);
            }
        }

        void Shutdown() override
        {
            if (_configWindow != nullptr)
                DestroyConfigWindow();
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

        void Quit() override
        {
            _running = false;
        }

        void Redraw()
        {
            _frameRenderer->BeginFrame();
            if (_ui != nullptr)
                _ui->Draw();

            bool wantConfig = _ui && _ui->IsConfigOpen();
            if (wantConfig && _configWindow == nullptr)
            {
                try
                {
                    EnsureConfigWindow();
                }
                catch (const std::exception& e)
                {
                    SDL_Log("EnsureConfigWindow failed: %s", e.what());
                    if (_ui)
                        _ui->SetConfigOpen(false);
                    wantConfig = false;
                }
            }

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

            if (wantConfig && _configWindow && _configRenderer)
            {
                bool open = _ui->IsConfigOpen();
                _configRenderer->BeginFrame();
                _ui->DrawConfigWindow(&open);
                if (open != _ui->IsConfigOpen())
                    _ui->SetConfigOpen(open);

                SDL_GPUCommandBuffer* cb2 =
                    SDL_AcquireGPUCommandBuffer(_device->get());
                SDL_GPUTexture* tex2 = nullptr;
                SDL_WaitAndAcquireGPUSwapchainTexture(
                    cb2, _configWindow, &tex2, nullptr, nullptr);
                _configRenderer->Render(cb2, tex2);
                SDL_SubmitGPUCommandBuffer(cb2);

                if (!open)
                    DestroyConfigWindow();
            }
            else if (!wantConfig && _configWindow != nullptr)
            {
                DestroyConfigWindow();
            }
        }

    private:
        static Sint32 ToClampedMs(Uint64 ns)
        {
            constexpr Uint64 kMaxNs =
                static_cast<Uint64>(std::numeric_limits<Sint32>::max()) *
                1'000'000ULL;
            if (ns > kMaxNs)
                return std::numeric_limits<Sint32>::max();
            return static_cast<Sint32>(ns / 1'000'000ULL);
        }

        Sint32 ComputeTimeoutMs(Uint64 now, bool pendingRedraw) const
        {
            Sint32 timeoutMs = -1;
            if (auto wait = _timer->GetTimeUntilNextEvent(now);
                wait.has_value())
                timeoutMs = ToClampedMs(*wait);
            if (pendingRedraw)
            {
                Uint64 earliest = _lastRedrawNs + _drawNs;
                Uint64 untilFrame = (earliest <= now) ? 0 : earliest - now;
                Sint32 frameMs = ToClampedMs(untilFrame);
                if (timeoutMs < 0 || frameMs < timeoutMs)
                    timeoutMs = frameMs;
            }
            return timeoutMs;
        }

        void HandleEvent(SDL_Event& event)
        {
            _frameRenderer->ProcessEvent(&event);
            if (_configRenderer)
                _configRenderer->ProcessEvent(&event);
            if (event.type == SDL_EVENT_QUIT)
                Quit();
            if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED)
            {
                if (event.window.windowID == SDL_GetWindowID(_window->get()))
                    Quit();
                else if (_configWindow &&
                         event.window.windowID == _configWindowID)
                    DestroyConfigWindow();
            }
        }

        void TryRedraw(Uint64 now, bool& pendingRedraw)
        {
            Uint64 earliest = _lastRedrawNs + _drawNs;
            if (now < earliest)
                return;

            Redraw();
            _lastRedrawNs = now;
            pendingRedraw = false;
        }

        std::shared_ptr<Preferences> _prefs;
        DockFactory _dockFactory;
        std::unique_ptr<ImGuiFrameRenderer> _frameRenderer;
        std::unique_ptr<Timer> _timer;
        std::unique_ptr<ImGuiIf> _imgui;
        std::unique_ptr<Sensors> _sensors;
        std::unique_ptr<Ui> _ui;
        std::unique_ptr<Dock> _dock;
        std::unique_ptr<SdlSession> _sdl;
        std::unique_ptr<DockWindow> _window;
        std::unique_ptr<GpuDevice> _device;
        bool _rendererInited = false;
        SDL_Window* _configWindow = nullptr;
        SDL_WindowID _configWindowID = 0;
        std::unique_ptr<ImGuiFrameRenderer> _configRenderer;
        bool _configRendererInited = false;
        Uint64 _drawNs = 0;
        Uint64 _lastRedrawNs = 0;
        bool _running = true;
    };

} // namespace

std::unique_ptr<App> CreateApp(const CliArgs& args)
{
    auto prefs = std::shared_ptr<Preferences>(CreatePreferences(args));
    auto dockFactory = CreateDockFactory(*prefs);
    auto timer = CreateTimer();
    auto imgui = CreateImGui();
    auto renderer = CreateImGuiFrameRenderer(*prefs);
    auto app = std::make_unique<ImGuiAppImpl>(std::move(dockFactory),
        std::move(renderer),
        prefs,
        std::move(timer),
        std::move(imgui));
    auto* appImpl = static_cast<ImGuiAppImpl*>(app.get());
    auto sensors = CreateSensors(*appImpl);
    auto ui = CreateUi(*appImpl, *sensors);
    appImpl->SetSensors(std::move(sensors));
    appImpl->SetUi(std::move(ui));
    return app;
}
