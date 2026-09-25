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

class Value
{
public:
    virtual ~Value() = default;

    enum class Type
    {
        String,
        Integer,
        Double,
        Boolean,
        StringList
    };

    virtual std::string GetName() const = 0;
    virtual Type GetType() const = 0;
    virtual std::optional<std::string> GetString() const = 0;
    virtual std::optional<int> GetInteger() const = 0;
    virtual std::optional<double> GetDouble() const = 0;
    virtual std::optional<bool> GetBoolean() const = 0;
    virtual std::optional<std::vector<std::string>> GetStringList() const = 0;
    virtual std::optional<std::set<std::string>> GetStringSet() const = 0;

    virtual void SetString(const std::string& value) = 0;
    virtual void SetInteger(int value) = 0;
    virtual void SetDouble(double value) = 0;
    virtual void SetBoolean(bool value) = 0;
    virtual void SetStringList(const std::vector<std::string>& value) = 0;
    virtual void SetStringSet(const std::set<std::string>& value) = 0;

    bool operator==(const Value& other) const
    {
        return GetName() == other.GetName();
    }
    bool operator!=(const Value& other) const
    {
        return !(*this == other);
    }
    bool operator<(const Value& other) const
    {
        return GetName() < other.GetName();
    }
    bool operator<=(const Value& other) const
    {
        return !(other < *this);
    }
    bool operator>(const Value& other) const
    {
        return other < *this;
    }
    bool operator>=(const Value& other) const
    {
        return !(*this < other);
    }
};

namespace std
{
    template <>
    struct less<std::unique_ptr<Value>>
    {
        bool operator()(const std::unique_ptr<Value>& a,
            const std::unique_ptr<Value>& b) const noexcept
        {
            if (!a)
                return static_cast<bool>(b);
            if (!b)
                return false;
            return *a < *b;
        }
    };
}

// Saved-preferences backend interface. Implementations live in their own
// <name>_preferences_impl.cc files. Missing keys return nullopt.
class Preferences
{
public:
    virtual ~Preferences() = default;

    virtual std::unique_ptr<Value> Get(const std::string& key) const = 0;
    virtual std::unique_ptr<Value> Add(const std::string& key) = 0;

    std::optional<std::string> GetString(const std::string& key) const;
    std::optional<int> GetInteger(const std::string& key) const;
    std::optional<double> GetDouble(const std::string& key) const;
    std::optional<bool> GetBoolean(const std::string& key) const;
    std::optional<std::vector<std::string>> GetStringList(
        const std::string& key) const;
    std::optional<std::set<std::string>> GetStringSet(
        const std::string& key) const;

    virtual std::set<std::unique_ptr<Value>> GetAll() const = 0;

    virtual void SetString(const std::string& key, const std::string& value);
    virtual void SetInteger(const std::string& key, int value);
    virtual void SetDouble(const std::string& key, double value);
    virtual void SetBoolean(const std::string& key, bool value);
    virtual void SetStringList(
        const std::string& key, const std::vector<std::string>& value);
    virtual void SetStringSet(
        const std::string& key, const std::set<std::string>& value);

    virtual void ClearAll();
};

extern std::unique_ptr<Preferences> CreateCliPreferences(const CliArgs& args);
extern std::unique_ptr<Preferences> CreateTomlPreferences();
extern void WriteTomlPreferences(const Preferences& prefs);
extern void WriteTomlPreferences(Preferences& prefs);
extern std::unique_ptr<Preferences> CreatePreferences(const CliArgs& args);

extern std::unique_ptr<Preferences> CreateDefaultPreferences();

extern std::shared_ptr<Preferences> CreateMapPreferences();

extern std::unique_ptr<Preferences> CreateCombinedPreferences(
    std::initializer_list<std::shared_ptr<Preferences>> sources);

extern std::unique_ptr<Value> CreateStringValue(
    const std::string& name, const std::string& s);

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
