#pragma once

#include <optional>
#include <string>

// Saved-preferences backend interface. Implementations live in their own
// <name>_preferences_impl.cc files. Missing keys return nullopt.
class Preferences {
public:
    virtual ~Preferences() = default;

    virtual std::optional<std::string> GetString(const std::string &key) const = 0;
    virtual std::optional<int> GetInteger(const std::string &key) const = 0;
};

// Typed accessors for the known preference keys.
class GlobalPreferencesFetcher {
public:
    static std::string GetSide(const Preferences &prefs)
    {
        return prefs.GetString("side").value_or("left");
    }

    static int GetSize(const Preferences &prefs)
    {
        return prefs.GetInteger("size").value_or(240);
    }

    static int GetMonitor(const Preferences &prefs)
    {
        return prefs.GetInteger("monitor").value_or(0);
    }
};
