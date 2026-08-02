#include "preferences.h"

std::string PreferencesFetcher::GetSide(const Preferences &prefs)
{
    return prefs.GetString("side", "left");
}

int PreferencesFetcher::GetSize(const Preferences &prefs)
{
    return prefs.GetInteger("size", 240);
}

int PreferencesFetcher::GetMonitor(const Preferences &prefs)
{
    return prefs.GetInteger("monitor", 0);
}
