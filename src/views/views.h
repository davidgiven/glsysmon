#pragma once

#include <map>
#include <memory>
#include <string>

#include "views/view.h"

class Preferences;
class Sensors;

class Views
{
public:
    explicit Views(const Preferences& prefs, Sensors& sensors);

    Views(const Views&) = delete;
    Views& operator=(const Views&) = delete;

    View* Get(const std::string& name);

    void Reset();

private:
    const Preferences& _prefs;
    Sensors& _sensors;
    std::map<std::string, std::unique_ptr<View>> _views;
};
