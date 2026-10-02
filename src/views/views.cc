#include "app.h"
#include "views.h"

#include "views/view.h"

namespace
{

    std::unique_ptr<View> CreateClockViewWithTimer(
        App& app, Sensors& sensors)
    {
        return CreateClockView(app, sensors);
    }

    std::unique_ptr<View> CreateCpuViewWithTimer(
        App& app, Sensors& sensors)
    {
        return CreateCpuView(app, sensors);
    }

    std::unique_ptr<View> CreateDiskViewWithTimer(
        App& app, Sensors& sensors)
    {
        return CreateDiskView(app, sensors);
    }

    std::unique_ptr<View> CreateHostnameViewWithTimer(
        App& app, Sensors& sensors)
    {
        return CreateHostnameView(app, sensors);
    }

    std::unique_ptr<View> CreateNetworkViewWithTimer(
        App& app, Sensors& sensors)
    {
        return CreateNetworkView(app, sensors);
    }

    std::unique_ptr<View> CreateTemperatureViewWithTimer(
        App& app, Sensors& sensors)
    {
        return CreateTemperatureView(app, sensors);
    }

    std::unique_ptr<View> CreateMemoryViewWithTimer(
        App& app, Sensors& sensors)
    {
        return CreateMemoryView(app, sensors);
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

Views::Views(App& app, Sensors& sensors):
    _app(app),
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
    std::unique_ptr<View> view = fIt->second(_app, _sensors);
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
