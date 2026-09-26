#pragma once

#include "poller.h"

#include <map>
#include <memory>
#include <string>

struct CpuSample
{
    float user;
    float system;
    float nice;
};

class CpuPoller : public Poller<CpuSample>
{
public:
    virtual ~CpuPoller() = default;
};

extern std::unique_ptr<CpuPoller> CreateCpuPoller(
    const std::string& procStatPath = "/proc/stat");
