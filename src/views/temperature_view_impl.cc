#include "views/view.h"

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

    class TemperatureViewImpl : public View
    {
    public:
        explicit TemperatureViewImpl(
            const Preferences& prefs, Sensors& sensors):
            _prefs(prefs),
            _sensor(sensors.CreateTemperatureSensor())
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
            const int minimum =
                _prefs.GetInteger("temperature.minimum").value_or(0);
            const int maximum =
                _prefs.GetInteger("temperature.maximum").value_or(100);
            const double yMin = static_cast<double>(minimum);
            const double yMax = static_cast<double>(maximum);
            const auto allowedSet = _prefs.GetStringSet("temperature.sensors");

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
                        char title[64];
                        std::snprintf(
                            title, sizeof(title), "##temperature%zu", ch);
                        const float width = ImGui::GetContentRegionAvail().x;
                        if (ImPlot::BeginPlot(title,
                                ImVec2(width, 40),
                                ImPlotFlags_NoTitle | ImPlotFlags_NoLegend |
                                    ImPlotFlags_NoMouseText |
                                    ImPlotFlags_NoInputs | ImPlotFlags_NoMenus |
                                    ImPlotFlags_NoBoxSelect |
                                    ImPlotFlags_NoFrame))
                        {
                            ImPlot::SetupAxes(nullptr,
                                nullptr,
                                ImPlotAxisFlags_NoDecorations,
                                ImPlotAxisFlags_NoDecorations);
                            ImPlot::SetupAxesLimits(
                                0, n, yMin, yMax, ImPlotCond_Always);
                            ImPlot::SetupFinish();
                            ImPlot::PlotLine(channelName.c_str(), samples, n);
                            ImPlot::EndPlot();
                        }

                        {
                            const float avail =
                                ImGui::GetContentRegionAvail().x;
                            const float textWidth =
                                ImGui::CalcTextSize(channelName.c_str()).x;
                            ImGui::SetCursorPosX(ImGui::GetCursorPosX() +
                                                 (avail - textWidth) * 0.5f);
                            ImGui::Text("%s", channelName.c_str());
                        }
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
