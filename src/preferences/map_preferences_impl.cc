#include "map_preferences_impl.h"

#include <map>
#include <memory>
#include <optional>
#include <string>

#include "components.h"
#include "preferences.h"

MapPreferencesImpl::MapPreferencesImpl(
    std::map<std::string, std::string> values):
    _values(std::move(values))
{
}

std::optional<std::string> MapPreferencesImpl::GetString(
    const std::string& key) const
{
    const auto it = _values.find(key);
    if (it == _values.end())
        return std::nullopt;
    return it->second;
}

void MapPreferencesImpl::Set(const std::string& key, const std::string& value)
{
    _values[key] = value;
}

std::unique_ptr<Preferences> CreateMapPreferences(
    std::map<std::string, std::string> values)
{
    return std::make_unique<MapPreferencesImpl>(std::move(values));
}

std::unique_ptr<Preferences> CreateMapPreferences()
{
    return std::make_unique<MapPreferencesImpl>(
        std::map<std::string, std::string>{});
}
