#include "preferences.h"

#include <map>
#include <memory>
#include <optional>
#include <set>
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

        std::unique_ptr<Value> Get(const std::string& key) const override
        {
            const auto it = _values.find(key);
            if (it == _values.end())
                return nullptr;
            return CreateStringValue(key, it->second);
        }

        std::set<std::string> GetAll() const override
        {
            std::set<std::string> result;
            for (const auto& [key, _] : _values)
                result.insert(key);
            return result;
        }

        void SetString(
            const std::string& key, const std::string& value) override
        {
            _values[key] = value;
        }

        void ClearAll() override
        {
            _values.clear();
        }

    private:
        std::map<std::string, std::string> _values;
    };

} // namespace

std::shared_ptr<Preferences> CreateMapPreferences()
{
    return std::make_shared<MapPreferencesImpl>(
        std::map<std::string, std::string>());
}
