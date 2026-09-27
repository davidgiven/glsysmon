#pragma once

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_gpu.h>

#include <memory>

// ImGui context, backends, and per-frame render pass. Owns everything ImGui so
// the app and the off-screen render harness share one implementation; callers
// only supply the SDL window/device and the target texture (swapchain or
// off-screen).
class ImGuiFrameRenderer
{
public:
    virtual ~ImGuiFrameRenderer() = default;

    // Creates the ImGui context, configures IO and style (DPI-scaled for the
    // window's display), and initializes the SDL3/SDLGPU3 backends. Must be
    // called once, after the GPU device is ready.
    virtual bool Init(SDL_GPUDevice* device,
        SDL_Window* window,
        SDL_GPUTextureFormat color_format) = 0;

    // Forwards an SDL event to the platform backend. Call while pumping events.
    virtual void ProcessEvent(const SDL_Event* event) = 0;

    // Begins an ImGui frame (renderer, platform, then core NewFrame).
    virtual void BeginFrame() = 0;

    // Ends the frame (ImGui::Render) and draws into `target` using the given
    // command buffer. A null target or degenerate display size skips the pass.
    virtual void Render(
        SDL_GPUCommandBuffer* command_buffer, SDL_GPUTexture* target) = 0;

    // Shuts down the backends and destroys the ImGui context.
    virtual void Shutdown() = 0;
};

extern std::unique_ptr<ImGuiFrameRenderer> CreateImGuiFrameRenderer();
