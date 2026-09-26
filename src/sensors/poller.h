#pragma once

#include <map>
#include <string>

// Fetches a piece of system data. Implementations live in poller_*.cc.
template <typename T>
class Poller
{
public:
    virtual ~Poller() = default;

    virtual std::map<std::string, T> Poll() = 0;
};
