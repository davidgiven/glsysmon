#include "views.h"

#include "views/view.h"

const std::map<std::string, Views::Factory> Views::_factories{
    {"ClockView",       static_cast<Factory>(&CreateClockView)      },
    {"CpuView",         static_cast<Factory>(&CreateCpuView)        },
    {"HostnameView",    static_cast<Factory>(&CreateHostnameView)   },
    {"TemperatureView", static_cast<Factory>(&CreateTemperatureView)},
};

Views::Views(const Preferences& prefs, Sensors& sensors):
    _prefs(prefs),
    _sensors(sensors)
{
}

std::vector<View*> Views::GetAllViews() const
{
    std::vector<View*> views;
    views.reserve(_factories.size());
    for (const auto& [name, _] : _factories)
        views.push_back(Get(name));
    return views;
}

View* Views::Get(const std::string& name) const
{
    auto it = _views.find(name);
    if (it != _views.end())
        return it->second.get();
    auto fIt = _factories.find(name);
    if (fIt == _factories.end())
        return nullptr;
    std::unique_ptr<View> view = fIt->second(_prefs, _sensors);
    View* ptr = view.get();
    _views.emplace(name, std::move(view));
    return ptr;
}

void Views::Inject(const std::string& name, std::unique_ptr<View> view)
{
    _views[name] = std::move(view);
}

void Views::Reset()
{
    _views.clear();
}
