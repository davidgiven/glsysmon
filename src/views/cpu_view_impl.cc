#include "views/view.h"

#include <imgui.h>
#include <implot.h>

#include <cstdio>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "components.h"
#include "preferences/preferences.h"
#include "sensors/sensors.h"

namespace
{

    class CpuViewImpl : public View
    {
    public:
        explicit CpuViewImpl(const Preferences& prefs, Sensors& sensors):
            _prefs(prefs),
            _sensors(&sensors)
        {
        }

        explicit CpuViewImpl(
            const Preferences& prefs, std::unique_ptr<CpuSensor> sensor):
            _prefs(prefs),
            _sensor(std::move(sensor))
        {
        }

        void Draw() override
        {
            CpuSensor& sensor = _sensor ? *_sensor : _sensors->GetCpuSensor();
            const std::size_t cpuCount = sensor.GetChannels();
            const std::size_t sampleCount = sensor.GetSampleCount();
            if (cpuCount == 0 || sampleCount == 0)
                return;
            for (std::size_t cpu = 0; cpu < cpuCount; ++cpu)
            {
                const CpuSample* samples = sensor.GetSamples(cpu);
                if (samples == nullptr)
                    continue;
                const int n = static_cast<int>(sampleCount);
                std::vector<float> values;
                values.resize(static_cast<std::size_t>(3) * n);
                for (int i = 0; i < n; ++i)
                {
                    values[static_cast<std::size_t>(0) * n + i] =
                        samples[i].user;
                    values[static_cast<std::size_t>(1) * n + i] =
                        samples[i].system;
                    values[static_cast<std::size_t>(2) * n + i] =
                        samples[i].nice;
                }
                const char* labels[] = {"user", "system", "nice"};
                char title[32];
                std::snprintf(title, sizeof(title), "##cpu%zu", cpu);
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
                    ImPlot::SetupAxesLimits(0, n, 0, 1, ImPlotCond_Always);
                    ImPlot::SetupFinish();
                    ImPlot::PlotBarGroups(labels,
                        values.data(),
                        3,
                        n,
                        1.0,
                        0.5,
                        ImPlotSpec(
                            ImPlotProp_Flags, ImPlotBarGroupsFlags_Stacked));
                    ImPlot::EndPlot();
                }
                ImPlot::PopStyleVar();
                ImGui::PopStyleVar();
            }
        }

        std::string GetName() const override
        {
            return "Cpu";
        }

    private:
        const Preferences& _prefs;
        std::unique_ptr<CpuSensor> _sensor;
        Sensors* _sensors = nullptr;
    };

} // namespace

std::unique_ptr<View> CreateCpuView(const Preferences& prefs, Sensors& sensors)
{
    return std::make_unique<CpuViewImpl>(prefs, sensors);
}

std::unique_ptr<View> CreateCpuView(
    const Preferences& prefs, std::unique_ptr<CpuSensor> sensor)
{
    return std::make_unique<CpuViewImpl>(prefs, std::move(sensor));
}
