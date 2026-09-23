#include "preferences.h"

#include <map>
#include <memory>
#include <optional>
#include <string>

namespace
{

    class MapPreferencesImpl : public Preferences
    {
    public:
        explicit MapPreferencesImpl(
            const std::map<std::string, std::string>& values):
            _values(values)
        {
        }

        std::optional<std::string> GetString(
            const std::string& key) const override
        {
            const auto it = _values.find(key);
            if (it == _values.end())
                return std::nullopt;
            return it->second;
        }

        void Set(const std::string& key, const std::string& value) override
        {
            _values[key] = value;
        }

    private:
        std::map<std::string, std::string> _values;
    };

} // namespace

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
