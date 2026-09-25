#include "views/view.h"
#include "views/view_graph_mixin.h"

#include <imgui.h>
#include <implot.h>

#include <cstdio>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <vector>

#include "preferences/preferences.h"
#include "sensors/sensors.h"
#include "views/style.h"

namespace
{

    class TemperatureViewImpl : public ViewGraphMixin
    {
    public:
        explicit TemperatureViewImpl(
            const Preferences& prefs, Sensors& sensors):
            _prefs(prefs),
            _sensor(sensors.CreateTemperatureSensor(GetPrefName()))
        {
        }

        explicit TemperatureViewImpl(const Preferences& prefs,
            std::unique_ptr<TemperatureSensor> sensor):
            _prefs(prefs),
            _sensor(std::move(sensor))
        {
        }

        void Draw() override
        {
            const std::size_t count = _sensor->GetChannels();
            const std::size_t sampleCount = _sensor->GetSampleCount();
            if (count == 0 || sampleCount == 0)
                return;
            const int yMin =
                _prefs.GetDouble("temperature.minimum").value_or(0);
            const int yMax =
                _prefs.GetDouble("temperature.maximum").value_or(100);
            const auto allowedSet = _prefs.GetStringSet("temperature.sensors");
            const int graphHeight =
                _prefs.GetInteger(GetPrefName() + ".graph_height").value_or(40);

            Style::GraphGroup("Temperature",
                [&]
                {
                    for (std::size_t ch = 0; ch < count; ++ch)
                    {
                        const std::string channelName =
                            _sensor->GetChannelName(ch);
                        if (allowedSet &&
                            allowedSet->find(channelName) == allowedSet->end())
                            continue;
                        const double* samples = _sensor->GetSamples(ch);
                        if (samples == nullptr)
                            continue;
                        const int n = static_cast<int>(sampleCount);
                        Style::DrawGraph(
                            channelName,
                            n,
                            yMin,
                            yMax,
                            [&]
                            {
                                ImPlot::PlotLine(
                                    channelName.c_str(), samples, n);
                            },
                            static_cast<float>(graphHeight));
                    }
                });
        }

        std::string GetHumanName() const override
        {
            return "Temperature";
        }

        std::string GetPrefName() const override
        {
            return "temperature";
        }

        std::vector<Sensor*> GetSensors() override
        {
            return {static_cast<Sensor*>(_sensor.get())};
        }

        std::vector<Sensor*> GetSensors() const override
        {
            return {static_cast<Sensor*>(_sensor.get())};
        }

        void DrawConfiguration(Preferences& preferences) override
        {
            ViewGraphMixin::DrawConfiguration(preferences);

            float minMax[2] = {static_cast<float>(preferences
                                       .GetInteger(GetPrefName() + ".minimum")
                                       .value_or(20)),
                static_cast<float>(
                    preferences.GetInteger(GetPrefName() + ".maximum")
                        .value_or(80))};
            if (ImGui::DragFloat2("Min/Max (°C)", minMax))
            {
                preferences.SetInteger(
                    GetPrefName() + ".minimum", static_cast<int>(minMax[0]));
                preferences.SetInteger(
                    GetPrefName() + ".maximum", static_cast<int>(minMax[1]));
            }

            auto allowedSet =
                *preferences.GetStringSet(GetPrefName() + ".sensors");
            bool changed = false;
            ImGuiStyle& style = ImGui::GetStyle();
            float window_visible_x2 =
                ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;
            for (size_t i = 0; i < _sensor->GetChannels(); i++)
            {
                auto name = _sensor->GetChannelName(i);
                ImGui::PushID(static_cast<int>(i));
                bool state = allowedSet.contains(name);
                float width = ImGui::CalcTextSize(name.c_str()).x +
                              style.FramePadding.x * 2.0f;
                if (ImGui::Selectable(name.c_str(), state, 0, ImVec2(width, 0)))
                {
                    if (state)
                        allowedSet.erase(name);
                    else
                        allowedSet.insert(name);
                    changed = true;
                }
                ImGui::PopID();
                if (i + 1 < _sensor->GetChannels())
                {
                    std::string nextName = _sensor->GetChannelName(i + 1);
                    float nextWidth = ImGui::CalcTextSize(nextName.c_str()).x +
                                      style.FramePadding.x * 2.0f;
                    float last_x2 = ImGui::GetItemRectMax().x;
                    float next_x2 = last_x2 + style.ItemSpacing.x + nextWidth;
                    if (next_x2 < window_visible_x2)
                        ImGui::SameLine();
                }
            }
            if (changed)
                preferences.SetStringSet(
                    GetPrefName() + ".sensors", allowedSet);
        }

    private:
        const Preferences& _prefs;
        std::unique_ptr<TemperatureSensor> _sensor;
    };

} // namespace

std::unique_ptr<View> CreateTemperatureView(
    const Preferences& prefs, Sensors& sensors)
{
    return std::make_unique<TemperatureViewImpl>(prefs, sensors);
}

std::unique_ptr<View> CreateTemperatureView(
    const Preferences& prefs, std::unique_ptr<TemperatureSensor> sensor)
{
    return std::make_unique<TemperatureViewImpl>(prefs, std::move(sensor));
}
