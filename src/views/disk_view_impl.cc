#include "views/view.h"
#include "views/view_graph_mixin.h"

#include <imgui.h>
#include <implot.h>

#include <algorithm>
#include <cstdio>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <vector>

#include "globals.h"
#include "preferences/preferences.h"
#include "sensors/sensors.h"
#include "views/style.h"

namespace
{

    class DiskViewImpl : public ViewGraphMixin
    {
    public:
        explicit DiskViewImpl(const Preferences& prefs, Sensors& sensors):
            _prefs(prefs),
            _sensor(sensors.CreateDiskSensor(GetPrefName()))
        {
        }

        explicit DiskViewImpl(
            const Preferences& prefs, std::unique_ptr<DiskSensor> sensor):
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
            const int graphHeight =
                _prefs.GetInteger(GetPrefName() + ".graph_height").value_or(40);
            auto allowedSet = _prefs.GetStringSet(GetPrefName() + ".devices");

            Style::GraphGroup("Disk",
                [&]
                {
                    for (std::size_t ch = 0; ch < count; ++ch)
                    {
                        const std::string channelName =
                            _sensor->GetChannelName(ch);
                        if (allowedSet->find(channelName) == allowedSet->end())
                            continue;

                        const DiskSample* samples = _sensor->GetSamples(ch);
                        if (samples == nullptr)
                            continue;
                        const int n = static_cast<int>(sampleCount);

                        double maxVal = 0;
                        for (int i = 0; i < n; ++i)
                            maxVal = std::max(
                                {maxVal, samples[i].rxBps, samples[i].txBps});
                        if (maxVal <= 0)
                            maxVal = 1;
                        double yMax = maxVal * 1.1;

                        auto plot = [&](const char* label,
                                        const double* values,
                                        int colIdx)
                        {
                            const ImVec4 col = ImPlot::GetColormapColor(colIdx);
                            ImPlot::PlotShaded(label,
                                values,
                                n,
                                0.0,
                                1,
                                0,
                                ImPlotSpec(ImPlotProp_LineColor,
                                    col,
                                    ImPlotProp_FillColor,
                                    col,
                                    ImPlotProp_Stride,
                                    sizeof(DiskSample)));
                            ImPlot::PlotLine(label,
                                values,
                                n,
                                1,
                                0,
                                ImPlotSpec(ImPlotProp_LineColor,
                                    col,
                                    ImPlotProp_Stride,
                                    sizeof(DiskSample)));
                        };

                        ImVec2 startPos = ImGui::GetCursorPos();
                        DrawGraph(
                            _prefs,
                            channelName,
                            n,
                            0,
                            yMax,
                            [&]
                            {
                                plot("rx", &samples[0].rxBps, 0);
                            },
                            static_cast<float>(graphHeight));
                        ImGui::SetCursorPos(startPos);
                        DrawGraph(
                            _prefs,
                            channelName,
                            n,
                            yMax,
                            0,
                            [&]
                            {
                                plot("tx", &samples[0].txBps, 1);
                            },
                            static_cast<float>(graphHeight));
                    }
                });
        }

        std::string GetHumanName() const override
        {
            return "Disk";
        }

        std::string GetPrefName() const override
        {
            return "disk";
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

            // Visible devices
            auto allowedSet =
                preferences.GetStringSet(GetPrefName() + ".devices")
                    .value_or(std::set<std::string>());

            bool changed = false;
            ImGuiStyle& style = ImGui::GetStyle();
            float window_visible_x2 =
                ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;

            for (size_t i = 0; i < _sensor->GetChannels(); ++i)
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
                    GetPrefName() + ".devices", allowedSet);
        }

    private:
        const Preferences& _prefs;
        std::unique_ptr<DiskSensor> _sensor;
    };

} // namespace

std::unique_ptr<View> CreateDiskView(const Preferences& prefs, Sensors& sensors)
{
    return std::make_unique<DiskViewImpl>(prefs, sensors);
}

std::unique_ptr<View> CreateDiskView(
    const Preferences& prefs, std::unique_ptr<DiskSensor> sensor)
{
    return std::make_unique<DiskViewImpl>(prefs, std::move(sensor));
}
