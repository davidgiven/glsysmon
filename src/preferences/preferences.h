#pragma once

#include <initializer_list>
#include <map>
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
    virtual std::optional<bool> GetBoolean(const std::string& key) const;
    virtual std::optional<std::vector<std::string>> GetStringList(
        const std::string& key) const;
    virtual std::optional<std::set<std::string>> GetStringSet(
        const std::string& key) const;

    virtual void SetString(const std::string& key, const std::string& value)
    {
        (void)key;
        (void)value;
    };
    virtual void SetInteger(const std::string& key, int value);
    virtual void SetDouble(const std::string& key, double value);
    virtual void SetBoolean(const std::string& key, bool value);
    virtual void SetStringList(
        const std::string& key, const std::vector<std::string>& value);
    virtual void SetStringSet(
        const std::string& key, const std::set<std::string>& value);
};

extern std::unique_ptr<Preferences> CreateCliPreferences(const CliArgs& args);
extern std::unique_ptr<Preferences> CreateTomlPreferences();
extern std::unique_ptr<Preferences> CreatePreferences(const CliArgs& args);

extern std::unique_ptr<Preferences> CreateDefaultPreferences();

extern std::shared_ptr<Preferences> CreateMapPreferences();

extern std::unique_ptr<Preferences> CreateCombinedPreferences(
    std::initializer_list<std::shared_ptr<Preferences>> sources);

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
