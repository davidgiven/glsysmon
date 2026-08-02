#pragma once

#include <functional>
#include <optional>
#include <string>
#include <vector>

// Command-line arguments, fed into the DI graph as a component-function
// parameter. Wrapped so the value is hashable (required by Fruit for
// component-function arguments).
struct CliArgs {
    std::vector<std::string> values;
};

inline bool operator==(const CliArgs &a, const CliArgs &b)
{
    return a.values == b.values;
}

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

// Annotation markers identifying the preference sources when both are bound to
// the Preferences interface (see CombinedPreferencesImpl).
struct CliPreference {};
struct TomlPreference {};

namespace std {

template <>
struct hash<CliArgs> {
    std::size_t operator()(const CliArgs &args) const
    {
        std::size_t h = args.values.size();
        for (const std::string &s : args.values)
            h ^= std::hash<std::string>()(s) + 0x9e3779b9 + (h << 6) + (h >> 2);
        return h;
    }
};

}  // namespace std
