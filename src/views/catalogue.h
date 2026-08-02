#pragma once

#include <fruit/fruit.h>

#include <map>
#include <string>

#include "view.h"

// Name-to-module registry of available system-monitor views; used to load the
// views to display at run time.
extern const std::map<std::string, fruit::Component<View> (*)()>&
GetViewCatalogue();
