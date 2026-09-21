#include "preferences.h"

#include <memory>
#include <optional>
#include <string>

#include "components.h"

namespace
{

    class CombinedPreferencesImpl : public Preferences
    {
    public:
        CombinedPreferencesImpl(std::unique_ptr<Preferences> cli,
            std::unique_ptr<Preferences> toml,
            std::unique_ptr<Preferences> defaults):
            _cli(std::move(cli)),
            _toml(std::move(toml)),
            _defaults(std::move(defaults))
        {
        }

        std::optional<std::string> GetString(
            const std::string& key) const override
        {
            if (auto value = _cli->GetString(key))
                return value;
            if (auto value = _toml->GetString(key))
                return value;
            return _defaults->GetString(key);
        }

        std::optional<int> GetInteger(const std::string& key) const override
        {
            if (auto value = _cli->GetInteger(key))
                return value;
            if (auto value = _toml->GetInteger(key))
                return value;
            return _defaults->GetInteger(key);
        }

        std::optional<std::vector<std::string>> GetStringList(
            const std::string& key) const override
        {
            if (auto value = _cli->GetStringList(key))
                return value;
            if (auto value = _toml->GetStringList(key))
                return value;
            return _defaults->GetStringList(key);
        }

    private:
        std::unique_ptr<Preferences> _cli;
        std::unique_ptr<Preferences> _toml;
        std::unique_ptr<Preferences> _defaults;
    };

} // namespace

std::unique_ptr<Preferences> CreatePreferences(const CliArgs& args)
{
    auto cli = CreateCliPreferences(args);
    auto toml = CreateTomlPreferences();
    auto defaults = CreateDefaultPreferences();
    return std::make_unique<CombinedPreferencesImpl>(
        std::move(cli), std::move(toml), std::move(defaults));
}
