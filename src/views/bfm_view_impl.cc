#include "views/view.h"

#include <imgui.h>

#include <SDL3/SDL_gpu.h>

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "display/bfm_gpu_bridge.h"
#include "preferences/preferences.h"
#include "sensors/cpu_poller.h"
#include "sensors/memory_poller.h"
#include "sensors/network_poller.h"
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

extern "C"
{
    extern int fish_traffic;
    void bfm_set_network_speed(int rx, int tx);
    void bfm_set_cpu_percent(int percent);
}

namespace
{
    static constexpr int FISH_MAX_SPEED = 8;

    class BubbleFishyMonViewImpl : public View
    {
    public:
        explicit BubbleFishyMonViewImpl(
            const Preferences& prefs, Sensors& sensors, Timer& timer):
            _prefs(prefs),
            _timer(timer),
            _memoryPoller(sensors.CreateMemoryPoller()),
            _networkPoller(sensors.CreateNetworkPoller()),
            _cpuPoller(sensors.CreateCpuPoller())
        {
            fish_traffic = 1;
            srand(0);
            bfm_glsysmon_init();
            {
                int w = GetWidth(_prefs);
                int h = GetHeight(_prefs);
                bfm_set_size(w, h);
            }
            _inited = true;
            double interval = GetUpdateInterval(_prefs);
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

            int width = GetWidth(_prefs);
            int height = GetHeight(_prefs);
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
            float interval = static_cast<float>(GetUpdateInterval(preferences));
            if (ImGui::InputFloat("Update frequency", &interval))
            {
                SetUpdateInterval(preferences, interval);
                interval = static_cast<float>(GetUpdateInterval(preferences));
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

        double GetUpdateInterval(const Preferences& prefs) const
        {
            double v = prefs.GetDouble(GetPrefName() + ".update_interval")
                           .value_or(10.0);
            if (v <= 0)
                v = 10.0;
            return v;
        }

        void SetUpdateInterval(Preferences& prefs, double value) const
        {
            if (value <= 0)
                value = 1;
            prefs.SetDouble(GetPrefName() + ".update_interval", value);
        }

        void Tick(Timer::Time t)
        {
            {
                auto data = _memoryPoller->PollCached();
                auto it = data.find("mem");
                if (it != data.end())
                {
                    bm.mem_used = static_cast<u_int64_t>(it->second.usedRam);
                    bm.mem_max = static_cast<u_int64_t>(it->second.totalRam);
                    if (bm.mem_max != 0)
                        bm.mem_percent = static_cast<unsigned int>(
                            (100 * bm.mem_used) / bm.mem_max);
                    else
                        bm.mem_percent = 0;
                    bm.swap_used = 0;
                    bm.swap_max = 0;
                    bm.swap_percent = 0;
                }
            }

            {
                auto netData = _networkPoller->PollCached();
                std::uint64_t sumRx = 0;
                std::uint64_t sumTx = 0;
                for (const auto& kv : netData)
                {
                    sumRx += static_cast<std::uint64_t>(kv.second.rxBps);
                    sumTx += static_cast<std::uint64_t>(kv.second.txBps);
                }
                int rxSpeed = 0;
                int txSpeed = 0;
                if (sumRx != 0)
                {
                    rxSpeed = static_cast<int>(FISH_MAX_SPEED * sumRx / _maxRx);
                    if (rxSpeed == 0)
                        rxSpeed = 1;
                    if (rxSpeed > FISH_MAX_SPEED)
                        rxSpeed = FISH_MAX_SPEED;
                    if (_maxRx < sumRx)
                    {
                        _maxRx = sumRx;
                        _rxCnt = 0;
                    }
                    else
                    {
                        if (++_rxCnt > 5)
                        {
                            _maxRx = sumRx;
                            if (_maxRx < 10)
                                _maxRx = 10;
                            _rxCnt = 0;
                        }
                    }
                }
                else
                {
                    rxSpeed = 0;
                }
                if (sumTx != 0)
                {
                    txSpeed = static_cast<int>(FISH_MAX_SPEED * sumTx / _maxTx);
                    if (txSpeed == 0)
                        txSpeed = 1;
                    if (txSpeed > FISH_MAX_SPEED)
                        txSpeed = FISH_MAX_SPEED;
                    if (_maxTx < sumTx)
                    {
                        _maxTx = sumTx;
                        _txCnt = 0;
                    }
                    else
                    {
                        if (++_txCnt > 5)
                        {
                            _maxTx = sumTx;
                            if (_maxTx < 10)
                                _maxTx = 10;
                            _txCnt = 0;
                        }
                    }
                }
                else
                {
                    txSpeed = 0;
                }
                bfm_set_network_speed(rxSpeed, txSpeed);
            }

            {
                auto cpuData = _cpuPoller->PollCached();
                float sum = 0.0f;
                for (const auto& kv : cpuData)
                    sum += kv.second.user + kv.second.system + kv.second.nice;
                sum /= cpuData.size();

                int cpuPercent = static_cast<int>(sum * 100.0f);
                if (cpuPercent < 0)
                    cpuPercent = 0;
                if (cpuPercent > 100)
                    cpuPercent = 100;
                bfm_set_cpu_percent(cpuPercent);
            }

            bubblemon_update(0);
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
        std::shared_ptr<MemoryPoller> _memoryPoller;
        std::shared_ptr<NetworkPoller> _networkPoller;
        std::shared_ptr<CpuPoller> _cpuPoller;
        std::uint64_t _maxRx = 10;
        std::uint64_t _maxTx = 10;
        int _rxCnt = 0;
        int _txCnt = 0;
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
