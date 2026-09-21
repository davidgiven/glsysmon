#pragma once

#include <memory>
#include <optional>
#include <set>
#include <string>
#include <vector>

struct CliArgs
{
    std::vector<std::string> values;
};

// Saved-preferences backend interface. Implementations live in their own
// <name>_preferences_impl.cc files. Missing keys return nullopt.
class Preferences
{
public:
    virtual ~Preferences() = default;

    virtual std::optional<std::string> GetString(
        const std::string& key) const = 0;
    virtual std::optional<int> GetInteger(const std::string& key) const;
    virtual std::optional<double> GetDouble(const std::string& key) const;
    virtual std::optional<std::vector<std::string>> GetStringList(
        const std::string& key) const;
    virtual std::optional<std::set<std::string>> GetStringSet(
        const std::string& key) const;
};

extern std::unique_ptr<Preferences> CreateDefaultPreferences();

// Typed accessors for the known preference keys.
class GlobalPreferencesFetcher
{
public:
    static std::string GetSide(const Preferences& prefs)
    {
        if (auto value = prefs.GetString("side"))
            return *value;
        return CreateDefaultPreferences()->GetString("side").value();
    }

    static int GetSize(const Preferences& prefs)
    {
        if (auto value = prefs.GetInteger("size"))
            return *value;
        return CreateDefaultPreferences()->GetInteger("size").value();
    }

    static int GetMonitor(const Preferences& prefs)
    {
        if (auto value = prefs.GetInteger("monitor"))
            return *value;
        return CreateDefaultPreferences()->GetInteger("monitor").value();
    }

    static std::vector<std::string> GetViews(const Preferences& prefs)
    {
        if (auto value = prefs.GetStringList("views"))
            return *value;
        return CreateDefaultPreferences()->GetStringList("views").value();
    }

    static double GetFps(const Preferences& prefs)
    {
        if (auto value = prefs.GetDouble("fps"))
            return *value;
        return CreateDefaultPreferences()->GetDouble("fps").value();
    }
};
