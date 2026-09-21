#include "preferences.h"

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

    private:
        static const std::map<std::string, std::string> _values;
    };

    const std::map<std::string, std::string> DefaultPreferencesImpl::_values{
        {"side",                        "left"                                          },
        {"size",                        "100"                                           },
        {"monitor",                     "0"                                             },
        {"views",                       "HostnameView,ClockView,CpuView,TemperatureView"},
        {"fps",                         "30"                                            },
        {"cpu.update_interval",         "5"                                             },
        {"temperature.update_interval", "1"                                             },
        {"temperature.minimum",         "0"                                             },
        {"temperature.maximum",         "100"                                           },
    };

} // namespace

std::unique_ptr<Preferences> CreateDefaultPreferences()
{
    return std::make_unique<DefaultPreferencesImpl>();
}
