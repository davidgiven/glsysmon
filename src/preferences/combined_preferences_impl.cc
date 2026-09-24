#include "preferences.h"

#include <initializer_list>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <vector>

namespace
{

    class CombinedPreferencesImpl : public Preferences
    {
    public:
        explicit CombinedPreferencesImpl(
            std::initializer_list<std::shared_ptr<Preferences>> sources):
            _sources(sources)
        {
        }

        std::optional<std::string> GetString(
            const std::string& key) const override
        {
            for (const auto& source : _sources)
            {
                if (auto value = source->GetString(key))
                    return value;
            }
            return std::nullopt;
        }

        std::optional<int> GetInteger(const std::string& key) const override
        {
            for (const auto& source : _sources)
            {
                if (auto value = source->GetInteger(key))
                    return value;
            }
            return std::nullopt;
        }

        std::optional<double> GetDouble(const std::string& key) const override
        {
            for (const auto& source : _sources)
            {
                if (auto value = source->GetDouble(key))
                    return value;
            }
            return std::nullopt;
        }

        std::optional<bool> GetBoolean(const std::string& key) const override
        {
            for (const auto& source : _sources)
            {
                if (auto value = source->GetBoolean(key))
                    return value;
            }
            return std::nullopt;
        }

        std::optional<std::vector<std::string>> GetStringList(
            const std::string& key) const override
        {
            for (const auto& source : _sources)
            {
                if (auto value = source->GetStringList(key))
                    return value;
            }
            return std::nullopt;
        }

        std::set<std::string> GetAll() const override
        {
            std::set<std::string> result;
            for (const auto& source : _sources)
            {
                const std::set<std::string> keys = source->GetAll();
                result.insert(keys.begin(), keys.end());
            }
            return result;
        }

        void SetString(
            const std::string& key, const std::string& value) override
        {
            if (!_sources.empty())
                _sources.front()->SetString(key, value);
        }

        void ClearAll() override
        {
            if (!_sources.empty())
                _sources.front()->ClearAll();
        }

    private:
        std::vector<std::shared_ptr<Preferences>> _sources;
    };

} // namespace

std::unique_ptr<Preferences> CreateCombinedPreferences(
    std::initializer_list<std::shared_ptr<Preferences>> sources)
{
    return std::make_unique<CombinedPreferencesImpl>(sources);
}

std::unique_ptr<Preferences> CreatePreferences(const CliArgs& args)
{
    return CreateCombinedPreferences(
        {std::shared_ptr<Preferences>(CreateCliPreferences(args)),
            std::shared_ptr<Preferences>(CreateTomlPreferences()),
            std::shared_ptr<Preferences>(CreateDefaultPreferences())});
}
