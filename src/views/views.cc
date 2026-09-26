#include "views.h"

#include "views/view.h"

namespace
{

    std::unique_ptr<View> CreateClockViewWithTimer(
        const Preferences& prefs, Sensors& sensors, Timer& timer)
    {
        (void)timer;
        return CreateClockView(prefs, sensors);
    }

    std::unique_ptr<View> CreateCpuViewWithTimer(
        const Preferences& prefs, Sensors& sensors, Timer& timer)
    {
        (void)timer;
        return CreateCpuView(prefs, sensors);
    }

    std::unique_ptr<View> CreateDiskViewWithTimer(
        const Preferences& prefs, Sensors& sensors, Timer& timer)
    {
        (void)timer;
        return CreateDiskView(prefs, sensors);
    }

    std::unique_ptr<View> CreateHostnameViewWithTimer(
        const Preferences& prefs, Sensors& sensors, Timer& timer)
    {
        (void)timer;
        return CreateHostnameView(prefs, sensors);
    }

    std::unique_ptr<View> CreateNetworkViewWithTimer(
        const Preferences& prefs, Sensors& sensors, Timer& timer)
    {
        (void)timer;
        return CreateNetworkView(prefs, sensors);
    }

    std::unique_ptr<View> CreateTemperatureViewWithTimer(
        const Preferences& prefs, Sensors& sensors, Timer& timer)
    {
        (void)timer;
        return CreateTemperatureView(prefs, sensors);
    }

    std::unique_ptr<View> CreateMemoryViewWithTimer(
        const Preferences& prefs, Sensors& sensors, Timer& timer)
    {
        (void)timer;
        return CreateMemoryView(prefs, sensors);
    }

} // namespace

const std::map<std::string, Views::Factory> Views::_factories{
    {"BubbleFishyMonView", &CreateBubbleFishyMonView      },
    {"ClockView",          &CreateClockViewWithTimer      },
    {"CpuView",            &CreateCpuViewWithTimer        },
    {"DiskView",           &CreateDiskViewWithTimer       },
    {"HostnameView",       &CreateHostnameViewWithTimer   },
    {"MemoryView",         &CreateMemoryViewWithTimer     },
    {"NetworkView",        &CreateNetworkViewWithTimer    },
    {"TemperatureView",    &CreateTemperatureViewWithTimer},
};

Views::Views(const Preferences& prefs, Sensors& sensors, Timer& timer):
    _prefs(prefs),
    _sensors(sensors),
    _timer(timer)
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

std::vector<std::string> Views::GetAvailableNames() const
{
    std::vector<std::string> names;
    names.reserve(_factories.size());
    for (const auto& [name, _] : _factories)
        names.push_back(name);
    return names;
}

View* Views::Get(const std::string& name) const
{
    auto it = _views.find(name);
    if (it != _views.end())
        return it->second.get();
    auto fIt = _factories.find(name);
    if (fIt == _factories.end())
        return nullptr;
    std::unique_ptr<View> view = fIt->second(_prefs, _sensors, _timer);
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
