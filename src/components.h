#pragma once

#include <fruit/fruit.h>

#include <vector>

#include "app.h"
#include "dock.h"
#include "preferences.h"
#include "ui.h"

// Fruit DI wiring. Each module provides its own Get*Component() next to its
// implementation class; the composition root (main.cpp) builds an Injector
// from GetAppComponent(). This is the only header that touches Fruit.
fruit::Component<DockFactory> GetDockComponent();
fruit::Component<Ui> GetUiComponent();
fruit::Component<App> GetAppComponent();
fruit::Component<fruit::Required<std::vector<std::string>>,
                 fruit::Annotated<CliPreference, Preferences>>
GetCliPreferencesComponent();
fruit::Component<fruit::Annotated<TomlPreference, Preferences>> GetTomlPreferencesComponent();
fruit::Component<fruit::Required<std::vector<std::string>>, Preferences>
GetPreferencesComponent();
