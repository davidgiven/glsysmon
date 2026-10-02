#pragma once

#include "app.h"

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "views/view.h"

class App;
class Sensors;

class Views
{
public:
    explicit Views(App& app, Sensors& sensors);

    Views(const Views&) = delete;
    Views& operator=(const Views&) = delete;

    std::vector<View*> GetAllViews() const;

    std::vector<std::string> GetAvailableNames() const;

    View* Get(const std::string& name) const;

    void Inject(const std::string& name, std::unique_ptr<View> view);

    void Reset();

private:
    using Factory = std::unique_ptr<View> (*)(App&, Sensors&);
    static const std::map<std::string, Factory> _factories;

    App& _app;
    Sensors& _sensors;
    mutable std::map<std::string, std::unique_ptr<View>> _views;
};
