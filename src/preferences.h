#pragma once

#include <string>

// Saved-preferences backend interface. Implementations live in their own
// <name>_preferences_impl.cc files.
class Preferences {
public:
    virtual ~Preferences() = default;

    virtual std::string GetString(const std::string &key,
                                  const std::string &fallback) const = 0;
    virtual int GetInteger(const std::string &key, int fallback) const = 0;
};

// Typed accessors for the known preference keys.
class PreferencesFetcher {
public:
    static std::string GetSide(const Preferences &prefs);
    static int GetSize(const Preferences &prefs);
    static int GetMonitor(const Preferences &prefs);
};
