#include "preferences.h"

#include <initializer_list>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "components.h"

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

        explicit CombinedPreferencesImpl(
            std::vector<std::shared_ptr<Preferences>> sources):
            _sources(std::move(sources))
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

    private:
        std::vector<std::shared_ptr<Preferences>> _sources;
    };

} // namespace

std::unique_ptr<Preferences> CreateCombinedPreferences(
    std::initializer_list<std::shared_ptr<Preferences>> sources)
{
    return std::make_unique<CombinedPreferencesImpl>(sources);
}

std::unique_ptr<Preferences> CreateCombinedPreferences(
    std::vector<std::shared_ptr<Preferences>> sources)
{
    return std::make_unique<CombinedPreferencesImpl>(std::move(sources));
}

std::unique_ptr<Preferences> CreatePreferences(const CliArgs& args)
{
    return CreateCombinedPreferences(
        {std::shared_ptr<Preferences>(CreateCliPreferences(args)),
            std::shared_ptr<Preferences>(CreateTomlPreferences()),
            std::shared_ptr<Preferences>(CreateDefaultPreferences())});
}
