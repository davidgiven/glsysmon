#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "views/view.h"

class Preferences;
class Sensors;

class Views
{
public:
    explicit Views(const Preferences& prefs, Sensors& sensors);

    Views(const Views&) = delete;
    Views& operator=(const Views&) = delete;

    std::vector<View*> GetAllViews() const;

    View* Get(const std::string& name) const;

    void Reset();

private:
    using Factory = std::unique_ptr<View> (*)(const Preferences&, Sensors&);
    static const std::map<std::string, Factory> _factories;

    const Preferences& _prefs;
    Sensors& _sensors;
    mutable std::map<std::string, std::unique_ptr<View>> _views;
};
