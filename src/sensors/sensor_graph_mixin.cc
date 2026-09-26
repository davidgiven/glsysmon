#include "sensor_graph_mixin.h"

#include <imgui.h>

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
    double interval = prefs.GetDouble(prefPrefix + ".update_interval")
                          .value_or(defaultInterval);
    if (interval <= 0)
        interval = defaultInterval;
    _interval = interval;
    _delta = static_cast<std::uint64_t>(1'000'000'000ULL / interval);
}

void SensorGraphMixinBase::DrawIntervalConfiguration(
    Preferences& preferences, const std::string& prefPrefix)
{
    float interval = static_cast<float>(
        preferences.GetDouble(prefPrefix + ".update_interval").value_or(2.0));
    if (ImGui::InputFloat("Update interval (Hz)", &interval))
    {
        preferences.SetDouble(prefPrefix + ".update_interval", interval);
    }
}
