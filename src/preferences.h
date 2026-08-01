#pragma once

#include <string>

// Saved-preferences backend interface. Implementations live in preferences.cc.
class Preferences {
public:
    virtual ~Preferences() = default;

    virtual std::string GetSide() const = 0;
    virtual int GetSize() const = 0;
    virtual int GetMonitor() const = 0;
};
