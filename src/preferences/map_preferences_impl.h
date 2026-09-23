#pragma once

#include <map>
#include <memory>
#include <string>

#include "preferences.h"

class MapPreferencesImpl : public Preferences
{
public:
    explicit MapPreferencesImpl(
        const std::map<std::string, std::string>& values);

    std::optional<std::string> GetString(const std::string& key) const override;

    void Set(const std::string& key, const std::string& value) override;

private:
    std::map<std::string, std::string> _values;
};

extern std::shared_ptr<Preferences> CreateMapPreferences(
    const std::map<std::string, std::string>& values);
extern std::shared_ptr<Preferences> CreateMapPreferences();
