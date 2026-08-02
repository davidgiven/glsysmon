#pragma once

#include <fruit/fruit.h>

#include "app.h"
#include "dock.h"
#include "imgui_frame_renderer.h"
#include "preferences.h"
#include "sensors/hostname_sensor.h"
#include "ui.h"
#include "view.h"

// Fruit DI wiring. Each module provides its own Get*Component() next to its
// implementation class; the composition root (main.cpp) builds an Injector
// from GetAppComponent(). This is the only header that touches Fruit.
extern fruit::Component<fruit::Required<CliArgs>, DockFactory>
GetDockComponent();
extern fruit::Component<fruit::Required<CliArgs>, Ui> GetUiComponent();
extern fruit::Component<View> GetViewComponent();
extern fruit::Component<HostnameSensor> GetHostnameSensorComponent();
extern fruit::Component<ImGuiFrameRenderer> GetImGuiFrameRendererComponent();
extern fruit::Component<App> GetAppComponent(CliArgs* args);
extern fruit::Component<fruit::Required<CliArgs>,
    fruit::Annotated<CliPreference, Preferences>>
GetCliPreferencesComponent();
extern fruit::Component<fruit::Annotated<TomlPreference, Preferences>>
GetTomlPreferencesComponent();
extern fruit::Component<fruit::Required<CliArgs>, Preferences>
GetPreferencesComponent();
