#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "views/view.h"

class Preferences;
class Sensors;
class Timer;

class Views
{
public:
    explicit Views(const Preferences& prefs, Sensors& sensors, Timer& timer);

    Views(const Views&) = delete;
    Views& operator=(const Views&) = delete;

    std::vector<View*> GetAllViews() const;

    std::vector<std::string> GetAvailableNames() const;

    View* Get(const std::string& name) const;

    void Inject(const std::string& name, std::unique_ptr<View> view);

    void Reset();

private:
    using Factory = std::unique_ptr<View> (*)(
        const Preferences&, Sensors&, Timer&);
    static const std::map<std::string, Factory> _factories;

    const Preferences& _prefs;
    Sensors& _sensors;
    Timer& _timer;
    mutable std::map<std::string, std::unique_ptr<View>> _views;
};
