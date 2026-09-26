#include "bfm_gpu_bridge.h"

static SDL_GPUDevice* g_bfmDevice = nullptr;

void BfmSetGpuDevice(SDL_GPUDevice* device)
{
    g_bfmDevice = device;
}

SDL_GPUDevice* BfmGetGpuDevice()
{
    return g_bfmDevice;
}
