#include "catalogue.h"

#include <map>
#include <string>

#include "components.h"
#include "sensors/sensors.h"
#include "view.h"

const std::map<std::string, ViewFactory>& GetViewCatalogue()
{
    using FactoryFn = std::unique_ptr<View> (*)(const Preferences&, Sensors&);
    static const std::map<std::string, ViewFactory> catalogue = {
        {"ClockView",    static_cast<FactoryFn>(CreateClockView)   },
        {"CpuView",      static_cast<FactoryFn>(CreateCpuView)     },
        {"HostnameView", static_cast<FactoryFn>(CreateHostnameView)},
    };
    return catalogue;
}
