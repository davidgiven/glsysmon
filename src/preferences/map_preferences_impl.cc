#include "map_preferences_impl.h"

#include <map>
#include <memory>
#include <optional>
#include <string>

#include "components.h"
#include "preferences.h"

MapPreferencesImpl::MapPreferencesImpl(
    const std::map<std::string, std::string>& values):
    _values(values)
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

std::shared_ptr<Preferences> CreateMapPreferences(
    const std::map<std::string, std::string>& values)
{
    return std::make_shared<MapPreferencesImpl>(values);
}

std::shared_ptr<Preferences> CreateMapPreferences()
{
    return std::make_shared<MapPreferencesImpl>(
        std::map<std::string, std::string>());
}
