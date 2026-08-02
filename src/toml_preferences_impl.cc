#include "preferences.h"

#include <fruit/fruit.h>
#include <toml++/toml.h>

#include <cstdlib>
#include <filesystem>

namespace {

std::string DefaultConfigPath()
{
    const char *xdg = std::getenv("XDG_CONFIG_HOME");
    if (xdg != nullptr && *xdg != '\0')
        return std::string(xdg) + "/glrellm/config.toml";
    const char *home = std::getenv("HOME");
    if (home != nullptr && *home != '\0')
        return std::string(home) + "/.config/glrellm/config.toml";
    return "glrellm.toml";
}

class TomlPreferencesImpl : public Preferences {
public:
    TomlPreferencesImpl()
    {
        const std::string path = DefaultConfigPath();
        std::error_code ec;
        if (std::filesystem::exists(path, ec)) {
            try {
                _table = toml::parse_file(path);
            } catch (const toml::parse_error &) {
                _table = toml::table{};
            }
        }
    }

    using Inject = TomlPreferencesImpl();

    std::optional<std::string> GetString(const std::string &key) const override
    {
        return _table[key].value<std::string>();
    }

    std::optional<int> GetInteger(const std::string &key) const override
    {
        return _table[key].value<int>();
    }

private:
    toml::table _table;
};

}  // namespace

fruit::Component<Preferences> GetPreferencesComponent()
{
    return fruit::createComponent().bind<Preferences, TomlPreferencesImpl>();
}
