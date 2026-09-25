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

#include "preferences/preferences.h"
#include "sensors/sensors.h"
#include "views/style.h"

namespace
{

    class NetworkViewImpl : public ViewGraphMixin
    {
    public:
        explicit NetworkViewImpl(const Preferences& prefs, Sensors& sensors):
            _prefs(prefs),
            _sensor(sensors.CreateNetworkSensor(GetPrefName()))
        {
        }

        explicit NetworkViewImpl(
            const Preferences& prefs, std::unique_ptr<NetworkSensor> sensor):
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
            auto allowedSet =
                _prefs.GetStringSet(GetPrefName() + ".interfaces");

            Style::GraphGroup("Network",
                [&]
                {
                    for (std::size_t ch = 0; ch < count; ++ch)
                    {
                        const std::string channelName =
                            _sensor->GetChannelName(ch);
                        if (allowedSet->find(channelName) == allowedSet->end())
                            continue;

                        const NetworkSample* samples = _sensor->GetSamples(ch);
                        if (samples == nullptr)
                            continue;
                        const int n = static_cast<int>(sampleCount);

                        double yMax =
                            _prefs.GetDouble(GetPrefName() + ".maximum")
                                .value_or(0);
                        if (yMax <= 0)
                        {
                            double maxVal = 0;
                            for (int i = 0; i < n; ++i)
                            {
                                maxVal = std::max({maxVal,
                                    samples[i].rxBps,
                                    samples[i].txBps});
                            }
                            if (maxVal <= 0)
                                maxVal = 1;
                            yMax = maxVal * 1.1;
                        }

                        std::vector<double> rx(n);
                        std::vector<double> txInv(n);
                        for (int i = 0; i < n; ++i)
                        {
                            rx[i] = samples[i].rxBps;
                            txInv[i] = yMax - samples[i].txBps;
                        }

                        Style::DrawGraph(
                            channelName,
                            n,
                            0,
                            yMax,
                            [&]
                            {
                                ImPlot::PlotShaded("rx", rx.data(), n, 0.0);
                                ImPlot::PlotLine("rx", rx.data(), n);
                                ImPlot::PlotShaded("tx", txInv.data(), n, yMax);
                                ImPlot::PlotLine("tx", txInv.data(), n);
                            },
                            static_cast<float>(graphHeight));
                    }
                });
        }

        std::string GetHumanName() const override
        {
            return "Network";
        }

        std::string GetPrefName() const override
        {
            return "network";
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

            static constexpr const char* kMaximumLabels[] = {
                "1MBps", "10Mbps", "100Mbps", "1GBps"};
            static constexpr double kMaximumValues[] = {
                1'000'000, 10'000'000, 100'000'000, 1'000'000'000};
            double currentMaximum =
                preferences.GetDouble(GetPrefName() + ".maximum").value_or(0);
            int maximumIndex = 0;
            double bestDiff = std::abs(currentMaximum - kMaximumValues[0]);
            for (int i = 1; i < 4; ++i)
            {
                double diff = std::abs(currentMaximum - kMaximumValues[i]);
                if (diff < bestDiff)
                {
                    bestDiff = diff;
                    maximumIndex = i;
                }
            }
            if (ImGui::SliderInt("Maximum (B/s)",
                    &maximumIndex,
                    0,
                    3,
                    kMaximumLabels[maximumIndex]))
            {
                preferences.SetDouble(
                    GetPrefName() + ".maximum", kMaximumValues[maximumIndex]);
            }

            auto allowedSet =
                preferences.GetStringSet(GetPrefName() + ".interfaces")
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
                    GetPrefName() + ".interfaces", allowedSet);
        }

    private:
        const Preferences& _prefs;
        std::unique_ptr<NetworkSensor> _sensor;
    };

} // namespace

std::unique_ptr<View> CreateNetworkView(
    const Preferences& prefs, Sensors& sensors)
{
    return std::make_unique<NetworkViewImpl>(prefs, sensors);
}

std::unique_ptr<View> CreateNetworkView(
    const Preferences& prefs, std::unique_ptr<NetworkSensor> sensor)
{
    return std::make_unique<NetworkViewImpl>(prefs, std::move(sensor));
}
