#pragma once

#include "app.h"

#include "poller.h"

#include <map>
#include <memory>
#include <string>

class App;

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

extern std::unique_ptr<CpuPoller> CreateCpuPoller(
    App& app, const std::string& procStatPath = "/proc/stat");
