#include "preferences.h"

#include <map>
#include <memory>
#include <string>

#include "components.h"
#include "map_preferences_impl.h"

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

} // namespace

std::unique_ptr<Preferences> CreateDefaultPreferences()
{
    return std::make_unique<MapPreferencesImpl>(kDefaultValues);
}
