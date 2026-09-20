#include "preferences.h"

#include <memory>
#include <toml++/toml.h>

#include <cstdlib>
#include <filesystem>

#include "components.h"

namespace
{

    std::string DefaultConfigPath()
    {
        const char* xdg = std::getenv("XDG_CONFIG_HOME");
        if (xdg != nullptr && *xdg != '\0')
            return std::string(xdg) + "/glsysmon/config.toml";
        const char* home = std::getenv("HOME");
        if (home != nullptr && *home != '\0')
            return std::string(home) + "/.config/glsysmon/config.toml";
        return "glsysmon.toml";
    }

    class TomlPreferencesImpl : public Preferences
    {
    public:
        TomlPreferencesImpl()
        {
            const std::string path = DefaultConfigPath();
            std::error_code ec;
            if (std::filesystem::exists(path, ec))
            {
                try
                {
                    _table = toml::parse_file(path);
                }
                catch (const toml::parse_error&)
                {
                    _table = toml::table{};
                }
            }
        }

        std::optional<std::string> GetString(
            const std::string& key) const override
        {
            return _table[key].value<std::string>();
        }

        std::optional<int> GetInteger(const std::string& key) const override
        {
            return _table[key].value<int>();
        }

        std::optional<std::vector<std::string>> GetStringList(
            const std::string& key) const override
        {
            const toml::array* array = _table[key].as_array();
            if (array == nullptr)
                return std::nullopt;
            std::vector<std::string> result;
            for (const toml::node& item : *array)
            {
                const toml::value<std::string>* value = item.as_string();
                if (value != nullptr)
                    result.push_back(value->get());
            }
            return result;
        }

    private:
        toml::table _table;
    };

} // namespace

std::unique_ptr<Preferences> CreateTomlPreferences()
{
    return std::make_unique<TomlPreferencesImpl>();
}
