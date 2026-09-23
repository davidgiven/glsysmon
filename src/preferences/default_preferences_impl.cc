#include "preferences.h"

#include <map>
#include <memory>
#include <optional>
#include <string>

namespace
{
    const std::map<std::string, std::string> kDefaultValues{
        {"side",                        "left"                                          },
        {"size",                        "100"                                           },
        {"monitor",                     "0"                                             },
        {"views",                       "HostnameView,ClockView,CpuView,TemperatureView"},
        {"fps",                         "30"                                            },
        {"cpu.update_interval",         "2"                                             },
        {"temperature.update_interval", "1"                                             },
        {"temperature.minimum",         "20"                                            },
        {"temperature.maximum",         "80"                                            },
        {"temperature.sensors",         "CPU"                                           },
    };

    class DefaultPreferencesImpl : public Preferences
    {
    public:
        explicit DefaultPreferencesImpl(
            const std::map<std::string, std::string>& values):
            _values(values)
        {
        }

        std::optional<std::string> GetString(
            const std::string& key) const override
        {
            const auto it = _values.find(key);
            if (it == _values.end())
                return std::nullopt;
            return it->second;
        }

        void Set(const std::string& key, const std::string& value) override
        {
            _values[key] = value;
        }

    private:
        std::map<std::string, std::string> _values;
    };

} // namespace

std::unique_ptr<Preferences> CreateDefaultPreferences()
{
    return std::make_unique<DefaultPreferencesImpl>(kDefaultValues);
}
