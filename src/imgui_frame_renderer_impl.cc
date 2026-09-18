#include "imgui_frame_renderer.h"

#include <imgui.h>

#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlgpu3.h>

#include <memory>

namespace
{

    constexpr SDL_FColor kClearColor = {0.08f, 0.08f, 0.10f, 1.0f};

    class ImGuiFrameRendererImpl : public ImGuiFrameRenderer
    {
    public:
        bool Init(SDL_GPUDevice* device,
            SDL_Window* window,
            SDL_GPUTextureFormat color_format) override
        {
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGuiIO& io = ImGui::GetIO();
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
            io.IniFilename =
                nullptr; // do not persist window layout to imgui.ini
            ImGui::StyleColorsDark();

            const float ui_scale =
                SDL_GetDisplayContentScale(SDL_GetDisplayForWindow(window));
            ImGuiStyle& style = ImGui::GetStyle();
            style.ScaleAllSizes(ui_scale);
            style.FontScaleDpi = ui_scale;

            ImGui_ImplSDL3_InitForSDLGPU(window);
            ImGui_ImplSDLGPU3_InitInfo init_info = {};
            init_info.Device = device;
            init_info.ColorTargetFormat = color_format;
            init_info.MSAASamples = SDL_GPU_SAMPLECOUNT_1;
            ImGui_ImplSDLGPU3_Init(&init_info);
            return true;
        }

        void ProcessEvent(const SDL_Event* event) override
        {
            ImGui_ImplSDL3_ProcessEvent(event);
        }

        void BeginFrame() override
        {
            ImGui_ImplSDLGPU3_NewFrame();
            ImGui_ImplSDL3_NewFrame();
            ImGui::NewFrame();
        }

        void Render(SDL_GPUCommandBuffer* command_buffer,
            SDL_GPUTexture* target) override
        {
            ImGui::Render();
            ImDrawData* draw_data = ImGui::GetDrawData();
            if (target == nullptr || draw_data->DisplaySize.x <= 0.0f ||
                draw_data->DisplaySize.y <= 0.0f)
                return;
            // Required before a render pass that draws ImGui.
            ImGui_ImplSDLGPU3_PrepareDrawData(draw_data, command_buffer);

            SDL_GPUColorTargetInfo target_info = {};
            target_info.texture = target;
            target_info.load_op = SDL_GPU_LOADOP_CLEAR;
            target_info.store_op = SDL_GPU_STOREOP_STORE;
            target_info.clear_color = kClearColor;
            target_info.mip_level = 0;
            target_info.layer_or_depth_plane = 0;
            target_info.cycle = false;
            SDL_GPURenderPass* render_pass = SDL_BeginGPURenderPass(
                command_buffer, &target_info, 1, nullptr);
            ImGui_ImplSDLGPU3_RenderDrawData(
                draw_data, command_buffer, render_pass);
            SDL_EndGPURenderPass(render_pass);
        }

        void Shutdown() override
        {
            ImGui_ImplSDL3_Shutdown();
            ImGui_ImplSDLGPU3_Shutdown();
            ImGui::DestroyContext();
        }
    };

} // namespace

std::unique_ptr<ImGuiFrameRenderer> CreateImGuiFrameRenderer()
{
    return std::make_unique<ImGuiFrameRendererImpl>();
}
