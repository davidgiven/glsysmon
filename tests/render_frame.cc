// Off-screen UI renderer shared by the visual-snapshot test harnesses: draws
// one frame of an injected Ui into an SDL_GPU texture and writes it as a PNG.

#include "render_frame.h"

#include <SDL3/SDL.h>
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image.h>
#include <stb_image_write.h>

#include <cstddef>
#include <cstring>
#include <filesystem>
#include <string>

#include "imgui_frame_renderer.h"
#include "ui.h"

namespace render_frame
{

    int RenderFrame(int width,
        int height,
        const std::string& output,
        Ui& ui,
        ImGuiFrameRenderer& frameRenderer)
    {
        if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_GAMEPAD))
        {
            SDL_Log("SDL_Init failed: %s", SDL_GetError());
            return 1;
        }

        if (height <= 0)
        {
            SDL_Rect bounds = {};
            if (!SDL_GetDisplayUsableBounds(SDL_GetPrimaryDisplay(), &bounds))
            {
                SDL_Log(
                    "SDL_GetDisplayUsableBounds failed: %s", SDL_GetError());
                SDL_Quit();
                return 1;
            }
            height = bounds.h;
        }

        // The window is only a platform-backend carrier for ImGui; nothing is
        // presented and the GPU never claims it.
        SDL_Window* window = SDL_CreateWindow(
            "glsysmon (offscreen)", width, height, SDL_WINDOW_HIDDEN);
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

        if (!frameRenderer.Init(
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
            frameRenderer.Shutdown();
            SDL_DestroyGPUDevice(device);
            SDL_DestroyWindow(window);
            SDL_Quit();
            return 1;
        }

        frameRenderer.BeginFrame();
        ui.Draw(window, "offscreen");

        SDL_GPUCommandBuffer* command_buffer =
            SDL_AcquireGPUCommandBuffer(device);
        frameRenderer.Render(command_buffer, offscreen);

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
        // The downloaded pixels are only valid once all submitted work
        // completes.
        SDL_WaitForGPUIdle(device);

        void* pixels = SDL_MapGPUTransferBuffer(device, transfer_buffer, false);
        if (pixels == nullptr)
        {
            SDL_Log("SDL_MapGPUTransferBuffer failed: %s", SDL_GetError());
            SDL_ReleaseGPUTransferBuffer(device, transfer_buffer);
            SDL_ReleaseGPUTexture(device, offscreen);
            frameRenderer.Shutdown();
            SDL_DestroyGPUDevice(device);
            SDL_DestroyWindow(window);
            SDL_Quit();
            return 1;
        }

        std::filesystem::create_directories(
            std::filesystem::path(output).parent_path());
        const int written = stbi_write_png(output.c_str(),
            pixel_width,
            pixel_height,
            4,
            pixels,
            pixel_width * 4);
        SDL_UnmapGPUTransferBuffer(device, transfer_buffer);
        SDL_ReleaseGPUTransferBuffer(device, transfer_buffer);
        SDL_ReleaseGPUTexture(device, offscreen);
        frameRenderer.Shutdown();
        SDL_DestroyGPUDevice(device);
        SDL_DestroyWindow(window);
        SDL_Quit();

        if (written == 0)
        {
            SDL_Log("stbi_write_png failed: %s", output.c_str());
            return 1;
        }
        SDL_Log(
            "wrote %s (%d x %d)", output.c_str(), pixel_width, pixel_height);
        return 0;
    }

    bool ImagesMatch(const char* actual, const char* expected)
    {
        int actual_width = 0;
        int actual_height = 0;
        int expected_width = 0;
        int expected_height = 0;
        stbi_uc* actual_pixels =
            stbi_load(actual, &actual_width, &actual_height, nullptr, 4);
        stbi_uc* expected_pixels =
            stbi_load(expected, &expected_width, &expected_height, nullptr, 4);
        if (actual_pixels == nullptr || expected_pixels == nullptr)
        {
            SDL_Log("failed to load %s or %s", actual, expected);
            stbi_image_free(actual_pixels);
            stbi_image_free(expected_pixels);
            return false;
        }
        const bool match = actual_width == expected_width &&
                           actual_height == expected_height &&
                           std::memcmp(actual_pixels,
                               expected_pixels,
                               static_cast<std::size_t>(actual_width) *
                                   actual_height * 4) == 0;
        stbi_image_free(actual_pixels);
        stbi_image_free(expected_pixels);
        return match;
    }

} // namespace render_frame
