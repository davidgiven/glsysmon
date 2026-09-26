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
            }
        }

        void Draw() override
        {
            SDL_GPUDevice* device = BfmGetGpuDevice();
            if (device == nullptr)
                return;

            EnsureGpuResources(device);
            if (_texture == nullptr || _transfer == nullptr)
                return;

            Upload(device);

            float availWidth = ImGui::GetContentRegionAvail().x;
            if (availWidth <= 0)
                return;

            int scale = (int)(availWidth / 56.0f);
            if (scale < 1)
                scale = 1;
            ImVec2 imgSize((float)(56 * scale), (float)(56 * scale));
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
        }

    private:
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
            if (device != _device)
            {
                DestroyGpuResources();
                _device = device;
            }
            if (_texture != nullptr)
                return;

            SDL_GPUTextureCreateInfo tci{};
            tci.type = SDL_GPU_TEXTURETYPE_2D;
            tci.format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM;
            tci.usage = SDL_GPU_TEXTUREUSAGE_SAMPLER;
            tci.width = 56;
            tci.height = 56;
            tci.layer_count_or_depth = 1;
            tci.num_levels = 1;
            tci.sample_count = SDL_GPU_SAMPLECOUNT_1;
            _texture = SDL_CreateGPUTexture(device, &tci);

            SDL_GPUTransferBufferCreateInfo tbi{};
            tbi.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
            tbi.size = 56 * 56 * 4;
            _transfer = SDL_CreateGPUTransferBuffer(device, &tbi);
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
        }

        void Upload(SDL_GPUDevice* device)
        {
            unsigned char* rgb = bfm_get_rgb_buf();
            void* mapped = SDL_MapGPUTransferBuffer(device, _transfer, false);
            if (mapped == nullptr)
                return;
            unsigned char* dst = static_cast<unsigned char*>(mapped);
            for (int i = 0; i < 56 * 56; ++i)
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
            src.pixels_per_row = 56;
            src.rows_per_layer = 56;
            SDL_GPUTextureRegion dstReg{};
            dstReg.texture = _texture;
            dstReg.mip_level = 0;
            dstReg.layer = 0;
            dstReg.x = 0;
            dstReg.y = 0;
            dstReg.z = 0;
            dstReg.w = 56;
            dstReg.h = 56;
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
    };

} // namespace

std::unique_ptr<View> CreateBubbleFishyMonView(
    const Preferences& prefs, Sensors& sensors, Timer& timer)
{
    return std::make_unique<BubbleFishyMonViewImpl>(prefs, sensors, timer);
}
