#pragma once

#include <functional>
#include <map>
#include <memory>
#include <string>

#include "view.h"

class Sensors;

using ViewFactory = std::function<std::unique_ptr<View>(Sensors&)>;

// Name-to-module registry of available system-monitor views; used to load the
// views to display at run time.
extern const std::map<std::string, ViewFactory>& GetViewCatalogue();
