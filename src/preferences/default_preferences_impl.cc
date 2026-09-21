#include "preferences.h"

#include <charconv>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "components.h"

namespace
{

    class DefaultPreferencesImpl : public Preferences
    {
    public:
        std::optional<std::string> GetString(
            const std::string& key) const override
        {
            const auto it = _values.find(key);
            if (it == _values.end())
                return std::nullopt;
            return it->second;
        }

        std::optional<int> GetInteger(const std::string& key) const override
        {
            const auto it = _values.find(key);
            if (it == _values.end())
                return std::nullopt;
            int value = 0;
            const char* begin = it->second.data();
            const char* end = begin + it->second.size();
            const auto result = std::from_chars(begin, end, value);
            if (result.ec != std::errc{} || result.ptr != end)
                return std::nullopt;
            return value;
        }

        std::optional<std::vector<std::string>> GetStringList(
            const std::string& key) const override
        {
            const auto it = _values.find(key);
            if (it == _values.end())
                return std::nullopt;
            std::vector<std::string> result;
            std::size_t start = 0;
            while (start <= it->second.size())
            {
                const std::size_t comma = it->second.find(',', start);
                const std::size_t end =
                    comma == std::string::npos ? it->second.size() : comma;
                result.push_back(it->second.substr(start, end - start));
                if (comma == std::string::npos)
                    break;
                start = comma + 1;
            }
            return result;
        }

    private:
        static const std::map<std::string, std::string> _values;
    };

    const std::map<std::string, std::string> DefaultPreferencesImpl::_values{
        {"side",                        "left"                          },
        {"size",                        "100"                           },
        {"monitor",                     "0"                             },
        {"views",                       "HostnameView,ClockView,CpuView"},
        {"fps",                         "30"                            },
        {"cpu.update_interval",         "5"                             },
        {"temperature.update_interval", "1"                             },
    };

} // namespace

std::unique_ptr<Preferences> CreateDefaultPreferences()
{
    return std::make_unique<DefaultPreferencesImpl>();
}
