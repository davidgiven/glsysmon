#pragma once

#include "poller.h"

#include <map>
#include <memory>
#include <string>

struct MemorySample
{
    long long totalRam;
    long long usedRam;
};

class MemoryPoller : public Poller<MemorySample>
{
public:
    virtual ~MemoryPoller() = default;
};

extern std::unique_ptr<MemoryPoller> CreateMemoryPoller(
    const std::string& procMemInfoPath = "/proc/meminfo");
