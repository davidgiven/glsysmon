#include "views/view.h"

#include <imgui.h>
#include <implot.h>

#include <cstdio>
#include <memory>
#include <optional>
#include <set>
#include <string>

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

        void Draw() override
        {
            TemperatureSensor& sensor = _sensors->GetTemperatureSensor();
            const std::size_t count = sensor.GetChannels();
            const std::size_t sampleCount = sensor.GetSampleCount();
            if (count == 0 || sampleCount == 0)
                return;
            const int minimum =
                _prefs.GetInteger("temperature.minimum").value_or(0);
            const int maximum =
                _prefs.GetInteger("temperature.maximum").value_or(100);
            const double yMin = static_cast<double>(minimum);
            const double yMax = static_cast<double>(maximum);
            const auto allowedSet = _prefs.GetStringSet("temperature.sensors");

            for (std::size_t ch = 0; ch < count; ++ch)
            {
                const std::string channelName = sensor.GetChannelName(ch);
                if (allowedSet &&
                    allowedSet->find(channelName) == allowedSet->end())
                    continue;
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
                    ImPlot::SetupAxesLimits(
                        0, n, yMin, yMax, ImPlotCond_Always);
                    ImPlot::SetupFinish();
                    ImPlot::PlotLine(channelName.c_str(), samples, n);
                    ImPlot::EndPlot();
                }
                ImPlot::PopStyleVar();
                ImGui::PopStyleVar();
                {
                    const float avail = ImGui::GetContentRegionAvail().x;
                    const float textWidth =
                        ImGui::CalcTextSize(channelName.c_str()).x;
                    ImGui::SetCursorPosX(
                        ImGui::GetCursorPosX() + (avail - textWidth) * 0.5f);
                    ImGui::Text("%s", channelName.c_str());
                }
            }
        }

        std::string GetName() const override
        {
            return "Temperature";
        }

    private:
        const Preferences& _prefs;
        Sensors* _sensors = nullptr;
    };

} // namespace

std::unique_ptr<View> CreateTemperatureView(
    const Preferences& prefs, Sensors& sensors)
{
    return std::make_unique<TemperatureViewImpl>(prefs, sensors);
}
