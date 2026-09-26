#include "views/view.h"

#include <imgui.h>

#include <SDL3/SDL_gpu.h>

#include <algorithm>
#include <cmath>
#include <memory>
#include <string>
#include <vector>

#include "display/bfm_gpu_bridge.h"
#include "preferences/preferences.h"
#include "sensors/sensors.h"
#include "timer.h"

extern "C"
{
#define GLSYSMON_BFM
#define ENABLE_FISH
#define ENABLE_DUCK
#define ENABLE_CPU
#define UPSIDE_DOWN_DUCK
#include "dep/bfm/include/bubblemon.h"
}

namespace
{

    class BubbleFishyMonViewImpl : public View
    {
    public:
        explicit BubbleFishyMonViewImpl(
            const Preferences& prefs, Sensors& /*sensors*/, Timer& timer):
            _prefs(prefs),
            _timer(timer)
        {
            srand(0);
            bfm_glsysmon_init();
            {
                int w = _prefs.GetInteger("bubbleFishyMon.width").value_or(56);
                int h = _prefs.GetInteger("bubbleFishyMon.height").value_or(56);
                if (w < 10)
                    w = 10;
                if (w > 256)
                    w = 256;
                if (h < 10)
                    h = 10;
                if (h > 256)
                    h = 256;
                bfm_set_size(w, h);
            }
            _inited = true;
            double interval = _prefs.GetDouble("bubbleFishyMon.update_interval")
                                  .value_or(10.0);
            if (interval <= 0)
                interval = 10.0;
            _interval = interval;
            _delta = static_cast<Timer::Time>(1'000'000'000.0 / _interval);
            Timer::Time now = _timer.Now();
            _scheduled = _timer.Schedule(now + _delta,
                [this](Timer::Time t)
                {
                    Tick(t);
                });
        }

        ~BubbleFishyMonViewImpl() override
        {
            if (_scheduled != 0)
                _timer.Cancel(_scheduled);
            if (BfmGetGpuDevice() != nullptr && BfmGetGpuDevice() == _device)
                DestroyGpuResources();
            else
            {
                _transfer = nullptr;
                _texture = nullptr;
                _device = nullptr;
                _texWidth = 0;
                _texHeight = 0;
            }
        }

        void Draw() override
        {
            SDL_GPUDevice* device = BfmGetGpuDevice();
            if (device == nullptr)
                return;

            int width = _prefs.GetInteger("bubbleFishyMon.width").value_or(56);
            int height =
                _prefs.GetInteger("bubbleFishyMon.height").value_or(56);
            if (width < 10)
                width = 10;
            if (width > 256)
                width = 256;
            if (height < 10)
                height = 10;
            if (height > 256)
                height = 256;
            int curW = 0;
            int curH = 0;
            bfm_get_size(&curW, &curH);
            if (curW != width || curH != height)
                bfm_set_size(width, height);

            EnsureGpuResources(device);
            if (_texture == nullptr || _transfer == nullptr)
                return;

            Upload(device);

            float availWidth = ImGui::GetContentRegionAvail().x;
            if (availWidth <= 0)
                return;

            int scale = (int)(availWidth / (float)width);
            if (scale < 1)
                scale = 1;
            ImVec2 imgSize((float)(width * scale), (float)(height * scale));
            float cursorX = ImGui::GetCursorPosX();
            ImGui::SetCursorPosX(cursorX + (availWidth - imgSize.x) * 0.5f);

            ImDrawList* dl = ImGui::GetWindowDrawList();
            dl->AddCallback(
                ImGui::GetPlatformIO().DrawCallback_SetSamplerNearest, nullptr);
            ImGui::Image((ImTextureID)(intptr_t)_texture, imgSize);
            dl->AddCallback(
                ImGui::GetPlatformIO().DrawCallback_SetSamplerLinear, nullptr);
        }

        std::string GetHumanName() const override
        {
            return "BubbleFishyMon";
        }

        std::string GetPrefName() const override
        {
            return "bubbleFishyMon";
        }

        std::vector<Sensor*> GetSensors() override
        {
            return {};
        }

        std::vector<Sensor*> GetSensors() const override
        {
            return {};
        }

        void DrawConfiguration(Preferences& preferences) override
        {
            float interval = static_cast<float>(
                preferences.GetDouble("bubbleFishyMon.update_interval")
                    .value_or(_interval));
            if (ImGui::InputFloat("Update frequency", &interval))
            {
                if (interval <= 0)
                    interval = 1;
                preferences.SetDouble(
                    "bubbleFishyMon.update_interval", interval);
                if (_scheduled != 0)
                    _timer.Cancel(_scheduled);
                _interval = interval;
                _delta = static_cast<Timer::Time>(1'000'000'000.0 / _interval);
                _scheduled = _timer.Schedule(_timer.Now() + _delta,
                    [this](Timer::Time t)
                    {
                        Tick(t);
                    });
            }

            float size[2] = {static_cast<float>(GetWidth(preferences)),
                static_cast<float>(GetHeight(preferences))};
            if (ImGui::DragFloat2("Size", size, 1.0f, 10.0f, 256.0f))
            {
                int w = static_cast<int>(std::lround(size[0]));
                int h = static_cast<int>(std::lround(size[1]));
                if (w < 10)
                    w = 10;
                if (w > 256)
                    w = 256;
                if (h < 10)
                    h = 10;
                if (h > 256)
                    h = 256;
                SetWidth(preferences, w);
                SetHeight(preferences, h);
                bfm_set_size(w, h);
            }
        }

    private:
        int GetWidth(const Preferences& prefs) const
        {
            int v = prefs.GetInteger(GetPrefName() + ".width").value_or(56);
            if (v < 10)
                v = 10;
            if (v > 256)
                v = 256;
            return v;
        }

        int GetHeight(const Preferences& prefs) const
        {
            int v = prefs.GetInteger(GetPrefName() + ".height").value_or(56);
            if (v < 10)
                v = 10;
            if (v > 256)
                v = 256;
            return v;
        }

        void SetWidth(Preferences& prefs, int value) const
        {
            if (value < 10)
                value = 10;
            if (value > 256)
                value = 256;
            prefs.SetInteger(GetPrefName() + ".width", value);
        }

        void SetHeight(Preferences& prefs, int value) const
        {
            if (value < 10)
                value = 10;
            if (value > 256)
                value = 256;
            prefs.SetInteger(GetPrefName() + ".height", value);
        }

        void Tick(Timer::Time t)
        {
            bfm_glsysmon_update(0);
            _scheduled = _timer.Schedule(t + _delta,
                [this](Timer::Time nt)
                {
                    Tick(nt);
                });
        }
        void EnsureGpuResources(SDL_GPUDevice* device)
        {
            int width = GetWidth(_prefs);
            int height = GetHeight(_prefs);
            if (device != _device)
            {
                DestroyGpuResources();
                _device = device;
            }
            if (_texture != nullptr &&
                (_texWidth != width || _texHeight != height))
            {
                if (_transfer)
                {
                    SDL_ReleaseGPUTransferBuffer(_device, _transfer);
                    _transfer = nullptr;
                }
                if (_texture)
                {
                    SDL_ReleaseGPUTexture(_device, _texture);
                    _texture = nullptr;
                }
                _texWidth = 0;
                _texHeight = 0;
            }
            if (_texture != nullptr)
                return;

            SDL_GPUTextureCreateInfo tci{};
            tci.type = SDL_GPU_TEXTURETYPE_2D;
            tci.format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM;
            tci.usage = SDL_GPU_TEXTUREUSAGE_SAMPLER;
            tci.width = static_cast<Uint32>(width);
            tci.height = static_cast<Uint32>(height);
            tci.layer_count_or_depth = 1;
            tci.num_levels = 1;
            tci.sample_count = SDL_GPU_SAMPLECOUNT_1;
            _texture = SDL_CreateGPUTexture(device, &tci);

            SDL_GPUTransferBufferCreateInfo tbi{};
            tbi.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
            tbi.size = static_cast<Uint32>(width * height * 4);
            _transfer = SDL_CreateGPUTransferBuffer(device, &tbi);
            _texWidth = width;
            _texHeight = height;
        }

        void DestroyGpuResources()
        {
            if (_device == nullptr)
                return;
            if (_transfer)
            {
                SDL_ReleaseGPUTransferBuffer(_device, _transfer);
                _transfer = nullptr;
            }
            if (_texture)
            {
                SDL_ReleaseGPUTexture(_device, _texture);
                _texture = nullptr;
            }
            _texWidth = 0;
            _texHeight = 0;
        }

        void Upload(SDL_GPUDevice* device)
        {
            int width = _texWidth;
            int height = _texHeight;
            if (width <= 0 || height <= 0)
                return;
            unsigned char* rgb = bfm_get_rgb_buf();
            void* mapped = SDL_MapGPUTransferBuffer(device, _transfer, false);
            if (mapped == nullptr)
                return;
            unsigned char* dst = static_cast<unsigned char*>(mapped);
            for (int i = 0; i < width * height; ++i)
            {
                dst[i * 4 + 0] = rgb[i * 3 + 0];
                dst[i * 4 + 1] = rgb[i * 3 + 1];
                dst[i * 4 + 2] = rgb[i * 3 + 2];
                dst[i * 4 + 3] = 255;
            }
            SDL_UnmapGPUTransferBuffer(device, _transfer);

            SDL_GPUCommandBuffer* cb = SDL_AcquireGPUCommandBuffer(device);
            if (cb == nullptr)
                return;
            SDL_GPUCopyPass* cp = SDL_BeginGPUCopyPass(cb);
            SDL_GPUTextureTransferInfo src{};
            src.transfer_buffer = _transfer;
            src.offset = 0;
            src.pixels_per_row = static_cast<Uint32>(width);
            src.rows_per_layer = static_cast<Uint32>(height);
            SDL_GPUTextureRegion dstReg{};
            dstReg.texture = _texture;
            dstReg.mip_level = 0;
            dstReg.layer = 0;
            dstReg.x = 0;
            dstReg.y = 0;
            dstReg.z = 0;
            dstReg.w = static_cast<Uint32>(width);
            dstReg.h = static_cast<Uint32>(height);
            dstReg.d = 1;
            SDL_UploadToGPUTexture(cp, &src, &dstReg, false);
            SDL_EndGPUCopyPass(cp);
            SDL_SubmitGPUCommandBuffer(cb);
        }

        const Preferences& _prefs;
        Timer& _timer;
        double _interval = 10.0;
        Timer::Time _delta = 100'000'000;
        Timer::Time _scheduled = 0;
        bool _inited = false;
        SDL_GPUDevice* _device = nullptr;
        SDL_GPUTexture* _texture = nullptr;
        SDL_GPUTransferBuffer* _transfer = nullptr;
        int _texWidth = 0;
        int _texHeight = 0;
    };

} // namespace

std::unique_ptr<View> CreateBubbleFishyMonView(
    const Preferences& prefs, Sensors& sensors, Timer& timer)
{
    return std::make_unique<BubbleFishyMonViewImpl>(prefs, sensors, timer);
}
