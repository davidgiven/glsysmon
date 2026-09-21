#include "view.h"

#include <imgui.h>
#include <implot.h>

#include <cstdio>
#include <memory>
#include <string>

#include "components.h"
#include "preferences/preferences.h"
#include "sensors/sensors.h"

namespace
{

    class TemperatureViewImpl : public View
    {
    public:
        explicit TemperatureViewImpl(
            const Preferences& prefs, Sensors& sensors):
            _prefs(prefs),
            _sensors(&sensors)
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
            TemperatureSensor& sensor =
                _sensor ? *_sensor : _sensors->GetTemperatureSensor();
            const std::size_t count = sensor.GetChannels();
            const std::size_t sampleCount = sensor.GetSampleCount();
            if (count == 0 || sampleCount == 0)
                return;
            for (std::size_t ch = 0; ch < count; ++ch)
            {
                const double* samples = sensor.GetSamples(ch);
                if (samples == nullptr)
                    continue;
                const int n = static_cast<int>(sampleCount);
                char title[64];
                std::snprintf(title, sizeof(title), "##temperature%zu", ch);
                ImGui::PushStyleVar(
                    ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
                ImPlot::PushStyleVar(
                    ImPlotStyleVar_PlotPadding, ImVec2(0.0f, 0.0f));
                const float width = ImGui::GetContentRegionAvail().x;
                if (ImPlot::BeginPlot(title,
                        ImVec2(width, 40),
                        ImPlotFlags_NoTitle | ImPlotFlags_NoLegend |
                            ImPlotFlags_NoMouseText | ImPlotFlags_NoInputs |
                            ImPlotFlags_NoMenus | ImPlotFlags_NoBoxSelect |
                            ImPlotFlags_NoFrame))
                {
                    ImPlot::SetupAxes(nullptr,
                        nullptr,
                        ImPlotAxisFlags_NoDecorations,
                        ImPlotAxisFlags_NoDecorations);
                    ImPlot::SetupAxesLimits(0, n, 0, 100, ImPlotCond_Always);
                    ImPlot::SetupFinish();
                    ImPlot::PlotLine(
                        sensor.GetChannelName(ch).c_str(), samples, n);
                    ImPlot::EndPlot();
                }
                ImPlot::PopStyleVar();
                ImGui::PopStyleVar();
                ImGui::Text("%s", sensor.GetChannelName(ch).c_str());
            }
        }

    private:
        const Preferences& _prefs;
        std::unique_ptr<TemperatureSensor> _sensor;
        Sensors* _sensors = nullptr;
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
