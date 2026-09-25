#include "preferences.h"

#include <map>
#include <memory>
#include <optional>
#include <set>
#include <stdexcept>
#include <string>

namespace
{
    const std::map<std::string, std::string> kDefaultValues{
        {"side",                        "left"                                          },
        {"size",                        "100"                                           },
        {"monitor",                     "0"                                             },
        {"views",                       "HostnameView,ClockView,CpuView,TemperatureView"},
        {"fps",                         "30"                                            },
        {"cpu.graph_height",            "40"                                            },
        {"cpu.update_interval",         "2"                                             },
        {"temperature.graph_height",    "40"                                            },
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

        std::unique_ptr<Value> Get(const std::string& key) const override
        {
            const auto it = _values.find(key);
            if (it == _values.end())
                return nullptr;
            return CreateStringValue(key, it->second);
        }

        std::set<std::unique_ptr<Value>> GetAll() const override
        {
            std::set<std::unique_ptr<Value>> result;
            for (const auto& [key, value] : _values)
                result.insert(CreateStringValue(key, value));
            return result;
        }

        void SetString(
            const std::string& key, const std::string& value) override
        {
            (void)key;
            (void)value;
            throw std::runtime_error("unsupported operation");
        }

    private:
        std::map<std::string, std::string> _values;
    };

} // namespace

std::unique_ptr<Preferences> CreateDefaultPreferences()
{
    return std::make_unique<DefaultPreferencesImpl>(kDefaultValues);
}
