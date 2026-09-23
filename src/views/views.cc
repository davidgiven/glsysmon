#include "views.h"

#include "components.h"

Views::Views(const Preferences& prefs, Sensors& sensors):
    _prefs(prefs),
    _sensors(sensors)
{
}

View* Views::Get(const std::string& name)
{
    auto it = _views.find(name);
    if (it != _views.end())
        return it->second.get();
    std::unique_ptr<View> view;
    if (name == "ClockView")
        view = CreateClockView(_prefs, _sensors);
    else if (name == "CpuView")
        view = CreateCpuView(_prefs, _sensors);
    else if (name == "HostnameView")
        view = CreateHostnameView(_prefs, _sensors);
    else if (name == "TemperatureView")
        view = CreateTemperatureView(_prefs, _sensors);
    else
        return nullptr;
    View* ptr = view.get();
    _views.emplace(name, std::move(view));
    return ptr;
}

void Views::Reset()
{
    _views.clear();
}
