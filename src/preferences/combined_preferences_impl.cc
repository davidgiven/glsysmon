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
            std::unique_ptr<Preferences> toml):
            _cli(std::move(cli)),
            _toml(std::move(toml))
        {
        }

        std::optional<std::string> GetString(
            const std::string& key) const override
        {
            if (auto value = _cli->GetString(key))
                return value;
            return _toml->GetString(key);
        }

        std::optional<int> GetInteger(const std::string& key) const override
        {
            if (auto value = _cli->GetInteger(key))
                return value;
            return _toml->GetInteger(key);
        }

        std::optional<std::vector<std::string>> GetStringList(
            const std::string& key) const override
        {
            if (auto value = _cli->GetStringList(key))
                return value;
            return _toml->GetStringList(key);
        }

    private:
        std::unique_ptr<Preferences> _cli;
        std::unique_ptr<Preferences> _toml;
    };

} // namespace

std::unique_ptr<Preferences> CreatePreferences(const CliArgs& args)
{
    auto cli = CreateCliPreferences(args);
    auto toml = CreateTomlPreferences();
    return std::make_unique<CombinedPreferencesImpl>(
        std::move(cli), std::move(toml));
}
