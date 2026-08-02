#include "preferences.h"

#include <fruit/fruit.h>

#include <optional>
#include <string>

#include "components.h"

namespace {

class CombinedPreferencesImpl : public Preferences {
public:
    CombinedPreferencesImpl(Preferences *cli, Preferences *toml)
        : _cli(cli), _toml(toml)
    {
    }

    using Inject = CombinedPreferencesImpl(
        fruit::Annotated<CliPreference, Preferences *>,
        fruit::Annotated<TomlPreference, Preferences *>);

    std::optional<std::string> GetString(const std::string &key) const override
    {
        if (auto value = _cli->GetString(key))
            return value;
        return _toml->GetString(key);
    }

    std::optional<int> GetInteger(const std::string &key) const override
    {
        if (auto value = _cli->GetInteger(key))
            return value;
        return _toml->GetInteger(key);
    }

private:
    Preferences *_cli;
    Preferences *_toml;
};

}  // namespace

fruit::Component<fruit::Required<CliArgs>, Preferences> GetPreferencesComponent()
{
    return fruit::createComponent()
        .install(GetCliPreferencesComponent)
        .install(GetTomlPreferencesComponent)
        .bind<Preferences, CombinedPreferencesImpl>();
}
