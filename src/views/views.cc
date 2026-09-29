#include "views.h"

#include "context.h"
#include "views/view.h"

namespace
{

    std::unique_ptr<View> CreateClockViewWithTimer(
        const Context& ctx, Sensors& sensors)
    {
        return CreateClockView(ctx, sensors);
    }

    std::unique_ptr<View> CreateCpuViewWithTimer(
        const Context& ctx, Sensors& sensors)
    {
        return CreateCpuView(ctx, sensors);
    }

    std::unique_ptr<View> CreateDiskViewWithTimer(
        const Context& ctx, Sensors& sensors)
    {
        return CreateDiskView(ctx, sensors);
    }

    std::unique_ptr<View> CreateHostnameViewWithTimer(
        const Context& ctx, Sensors& sensors)
    {
        return CreateHostnameView(ctx, sensors);
    }

    std::unique_ptr<View> CreateNetworkViewWithTimer(
        const Context& ctx, Sensors& sensors)
    {
        return CreateNetworkView(ctx, sensors);
    }

    std::unique_ptr<View> CreateTemperatureViewWithTimer(
        const Context& ctx, Sensors& sensors)
    {
        return CreateTemperatureView(ctx, sensors);
    }

    std::unique_ptr<View> CreateMemoryViewWithTimer(
        const Context& ctx, Sensors& sensors)
    {
        return CreateMemoryView(ctx, sensors);
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

Views::Views(const Context& ctx, Sensors& sensors):
    _context(ctx),
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
    std::unique_ptr<View> view = fIt->second(_context, _sensors);
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
