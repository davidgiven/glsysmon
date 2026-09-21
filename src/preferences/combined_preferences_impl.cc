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
            std::initializer_list<std::unique_ptr<Preferences>> sources)
        {
            for (auto& source : sources)
            {
                _sources.push_back(std::move(
                    const_cast<std::unique_ptr<Preferences>&>(source)));
            }
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
        std::vector<std::unique_ptr<Preferences>> _sources;
    };

} // namespace

std::unique_ptr<Preferences> CreatePreferences(const CliArgs& args)
{
    return std::make_unique<CombinedPreferencesImpl>(
        std::initializer_list<std::unique_ptr<Preferences>>{
            CreateCliPreferences(args),
            CreateTomlPreferences(),
            CreateDefaultPreferences()});
}
