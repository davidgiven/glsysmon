#pragma once

#include <map>
#include <memory>
#include <string>

#include "preferences.h"

class MapPreferencesImpl : public Preferences
{
public:
    explicit MapPreferencesImpl(std::map<std::string, std::string> values);

    std::optional<std::string> GetString(const std::string& key) const override;

    void Set(const std::string& key, const std::string& value);

private:
    std::map<std::string, std::string> _values;
};

extern std::unique_ptr<Preferences> CreateMapPreferences(
    std::map<std::string, std::string> values);
extern std::unique_ptr<Preferences> CreateMapPreferences();
