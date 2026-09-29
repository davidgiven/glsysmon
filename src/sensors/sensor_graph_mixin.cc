#include "sensor_graph_mixin.h"

#include <imgui.h>

#include "context.h"
#include "imguiif.h"
#include "preferences/preferences.h"

void SensorGraphMixinBase::InitGraphBase(const Preferences& prefs,
    const std::string& prefPrefix,
    std::size_t& sampleCount,
    double defaultInterval)
{
    int size = GlobalPreferencesFetcher::GetSize(prefs);
    if (size <= 0)
        size = 1;
    sampleCount = static_cast<std::size_t>(size);
    double interval = GetUpdateInterval(prefs, prefPrefix, defaultInterval);
    if (interval <= 0)
        interval = defaultInterval;
    _interval = interval;
    _delta = static_cast<std::uint64_t>(1'000'000'000ULL / interval);
}

void SensorGraphMixinBase::InitGraphBase(const Context& ctx,
    const std::string& prefPrefix,
    std::size_t& sampleCount,
    double defaultInterval)
{
    InitGraphBase(ctx.preferences, prefPrefix, sampleCount, defaultInterval);
}

void SensorGraphMixinBase::DrawIntervalConfiguration(
    ImGuiIf& imgui, Preferences& preferences, const std::string& prefPrefix)
{
    float interval =
        static_cast<float>(GetUpdateInterval(preferences, prefPrefix, 2.0));
    if (imgui.InputFloat("Update interval (Hz)", &interval))
    {
        SetUpdateInterval(preferences, prefPrefix, interval);
    }
}

double SensorGraphMixinBase::GetUpdateInterval(const Preferences& prefs,
    const std::string& prefPrefix,
    double defaultInterval) const
{
    return prefs.GetDouble(prefPrefix + ".update_interval")
        .value_or(defaultInterval);
}

void SensorGraphMixinBase::SetUpdateInterval(
    Preferences& prefs, const std::string& prefPrefix, double value) const
{
    prefs.SetDouble(prefPrefix + ".update_interval", value);
}
