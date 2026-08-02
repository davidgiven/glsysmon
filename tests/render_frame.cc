// Renders one frame of the app's UI into an SDL_GPU off-screen texture and
// saves it as a PNG for visual inspection.
//
// Usage: render_frame [--width=N] [--height=N] [--output=PATH]

#include <SDL3/SDL.h>
#include <fruit/fruit.h>
#include <stb_image_write.h>

#include <charconv>
#include <filesystem>
#include <optional>
#include <string>

#include "components.h"
#include "imgui_frame_renderer.h"

namespace
{

    struct Options
    {
        int width = 240;
        std::optional<int> height;
        std::string output = ".obj/tests/render_frame.png";
    };

    std::optional<int> ParseInt(const std::string& text)
    {
        int value = 0;
        const char* begin = text.data();
        const char* end = begin + text.size();
        const auto result = std::from_chars(begin, end, value);
        if (result.ec != std::errc{} || result.ptr != end)
            return std::nullopt;
        return value;
    }

    bool ParseArgs(int argc, char** argv, Options* options)
    {
        for (int i = 1; i < argc; ++i)
        {
            const std::string arg = argv[i];
            if (arg.rfind("--width=", 0) == 0)
            {
                if (auto value = ParseInt(arg.substr(8)))
                    options->width = *value;
                else
                    return false;
            }
            else if (arg.rfind("--height=", 0) == 0)
            {
                if (auto value = ParseInt(arg.substr(9)))
                    options->height = *value;
                else
                    return false;
            }
            else if (arg.rfind("--output=", 0) == 0)
            {
                options->output = arg.substr(9);
            }
            else
            {
                return false;
            }
        }
        return true;
    }

    fruit::Component<Ui, ImGuiFrameRenderer> GetTestComponents()
    {
        return fruit::createComponent()
            .install(GetUiComponent)
            .install(GetImGuiFrameRendererComponent);
    }

} // namespace

int main(int argc, char** argv)
{
    Options options;
    if (!ParseArgs(argc, argv, &options))
    {
        SDL_Log("usage: %s [--width=N] [--height=N] [--output=PATH]", argv[0]);
        return 1;
    }

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_GAMEPAD))
    {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return 1;
    }

    if (!options.height)
    {
        SDL_Rect bounds = {};
        if (!SDL_GetDisplayUsableBounds(SDL_GetPrimaryDisplay(), &bounds))
        {
            SDL_Log("SDL_GetDisplayUsableBounds failed: %s", SDL_GetError());
            SDL_Quit();
            return 1;
        }
        options.height = bounds.h;
    }

    // The window is only a platform-backend carrier for ImGui; nothing is
    // presented and the GPU never claims it.
    SDL_Window* window = SDL_CreateWindow("glrellm (offscreen)",
        options.width,
        *options.height,
        SDL_WINDOW_HIDDEN);
    if (window == nullptr)
    {
        SDL_Log("SDL_CreateWindow failed: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_GPUDevice* device =
        SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV, false, nullptr);
    if (device == nullptr)
    {
        SDL_Log("SDL_CreateGPUDevice failed: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    fruit::Injector<Ui, ImGuiFrameRenderer> injector(GetTestComponents);
    Ui* ui = injector.get<Ui*>();
    ImGuiFrameRenderer* frame_renderer = injector.get<ImGuiFrameRenderer*>();
    if (!frame_renderer->Init(
            device, window, SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM))
    {
        SDL_Log("ImGuiFrameRenderer::Init failed");
        SDL_DestroyGPUDevice(device);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    int pixel_width = 0;
    int pixel_height = 0;
    SDL_GetWindowSizeInPixels(window, &pixel_width, &pixel_height);

    SDL_GPUTextureCreateInfo texture_info = {};
    texture_info.type = SDL_GPU_TEXTURETYPE_2D;
    texture_info.format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM;
    texture_info.usage =
        SDL_GPU_TEXTUREUSAGE_COLOR_TARGET | SDL_GPU_TEXTUREUSAGE_SAMPLER;
    texture_info.width = pixel_width;
    texture_info.height = pixel_height;
    texture_info.layer_count_or_depth = 1;
    texture_info.num_levels = 1;
    texture_info.sample_count = SDL_GPU_SAMPLECOUNT_1;
    SDL_GPUTexture* offscreen = SDL_CreateGPUTexture(device, &texture_info);
    if (offscreen == nullptr)
    {
        SDL_Log("SDL_CreateGPUTexture failed: %s", SDL_GetError());
        frame_renderer->Shutdown();
        SDL_DestroyGPUDevice(device);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    frame_renderer->BeginFrame();
    ui->Draw(window, "offscreen");

    SDL_GPUCommandBuffer* command_buffer = SDL_AcquireGPUCommandBuffer(device);
    frame_renderer->Render(command_buffer, offscreen);

    SDL_GPUTransferBufferCreateInfo transfer_info = {};
    transfer_info.usage = SDL_GPU_TRANSFERBUFFERUSAGE_DOWNLOAD;
    transfer_info.size = pixel_width * pixel_height * 4;
    SDL_GPUTransferBuffer* transfer_buffer =
        SDL_CreateGPUTransferBuffer(device, &transfer_info);

    SDL_GPUCopyPass* copy_pass = SDL_BeginGPUCopyPass(command_buffer);
    SDL_GPUTextureRegion source_region = {};
    source_region.texture = offscreen;
    source_region.mip_level = 0;
    source_region.layer = 0;
    source_region.x = 0;
    source_region.y = 0;
    source_region.z = 0;
    source_region.w = pixel_width;
    source_region.h = pixel_height;
    source_region.d = 1;
    SDL_GPUTextureTransferInfo destination = {};
    destination.transfer_buffer = transfer_buffer;
    destination.offset = 0;
    destination.pixels_per_row = pixel_width;
    destination.rows_per_layer = pixel_height;
    SDL_DownloadFromGPUTexture(copy_pass, &source_region, &destination);
    SDL_EndGPUCopyPass(copy_pass);

    SDL_SubmitGPUCommandBuffer(command_buffer);
    // The downloaded pixels are only valid once all submitted work completes.
    SDL_WaitForGPUIdle(device);

    void* pixels = SDL_MapGPUTransferBuffer(device, transfer_buffer, false);
    if (pixels == nullptr)
    {
        SDL_Log("SDL_MapGPUTransferBuffer failed: %s", SDL_GetError());
        SDL_ReleaseGPUTransferBuffer(device, transfer_buffer);
        SDL_ReleaseGPUTexture(device, offscreen);
        frame_renderer->Shutdown();
        SDL_DestroyGPUDevice(device);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    std::filesystem::create_directories(
        std::filesystem::path(options.output).parent_path());
    const int written = stbi_write_png(options.output.c_str(),
        pixel_width,
        pixel_height,
        4,
        pixels,
        pixel_width * 4);
    SDL_UnmapGPUTransferBuffer(device, transfer_buffer);
    SDL_ReleaseGPUTransferBuffer(device, transfer_buffer);
    SDL_ReleaseGPUTexture(device, offscreen);
    frame_renderer->Shutdown();
    SDL_DestroyGPUDevice(device);
    SDL_DestroyWindow(window);
    SDL_Quit();

    if (written == 0)
    {
        SDL_Log("stbi_write_png failed: %s", options.output.c_str());
        return 1;
    }
    SDL_Log("wrote %s (%d x %d)",
        options.output.c_str(),
        pixel_width,
        pixel_height);
    return 0;
}
