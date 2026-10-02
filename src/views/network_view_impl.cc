#include "app.h"
#include "views/view.h"
#include "views/view_graph_mixin.h"

#include <imgui.h>
#include <implot.h>

#include <algorithm>
#include <cstdio>
#include <format>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <vector>

#include "globals.h"
#include "imguiif.h"
#include "preferences/preferences.h"
#include "sensors/sensors.h"
#include "utils.h"
#include "views/style.h"

namespace
{

    class NetworkViewImpl : public ViewGraphMixin
    {
    public:
        explicit NetworkViewImpl(App& app, Sensors& sensors):
            _app(app),
            _sensor(sensors.CreateNetworkSensor(GetPrefName()))
        {
        }

        explicit NetworkViewImpl(
            App& app, std::unique_ptr<NetworkSensor> sensor):
            _app(app),
            _sensor(std::move(sensor))
        {
        }

        void Draw() override
        {
            const std::size_t count = _sensor->GetChannels();
            const std::size_t sampleCount = _sensor->GetSampleCount();
            if (count == 0 || sampleCount == 0)
                return;
            const int graphHeight = GetGraphHeight(_app.GetPreferences());
            const bool showNumbers = GetShowNumbers(_app.GetPreferences());
            auto allowedSet = GetInterfaces(_app.GetPreferences());

            Style::GraphGroup(_app.GetImGui(),
                "Network",
                [&]
                {
                    for (std::size_t ch = 0; ch < count; ++ch)
                    {
                        const std::string channelName =
                            _sensor->GetChannelName(ch);
                        if (allowedSet.find(channelName) == allowedSet.end())
                            continue;

                        const NetworkSample* samples = _sensor->GetSamples(ch);
                        if (samples == nullptr)
                            continue;
                        const int n = static_cast<int>(sampleCount);

                        double yMax = GetMaximum(_app.GetPreferences());
                        if (yMax <= 0)
                        {
                            double maxVal = 0;
                            for (int i = 0; i < n; ++i)
                                maxVal = std::max({maxVal,
                                    samples[i].rxBps,
                                    samples[i].txBps});
                            if (maxVal <= 0)
                                maxVal = 1;
                            yMax = maxVal * 1.1;
                        }

                        std::string subtitle;
                        if (showNumbers)
                        {
                            const NetworkSample& last =
                                samples[sampleCount - 1];
                            subtitle = std::format("RX {}\nTX {}",
                                FormatBinary(last.rxBps, "B/s"),
                                FormatBinary(last.txBps, "B/s"));
                        }

                        DrawGraph(
                            _app.GetImGui(),
                            _app.GetPreferences(),
                            channelName,
                            subtitle,
                            n,
                            0,
                            yMax,
                            [&]
                            {
                                ImVec4 col = _app.GetImGui().GetColormapColor(0);
                                _app.GetImGui().PlotShaded("rx",
                                    &samples[0].rxBps,
                                    n,
                                    0.0,
                                    1,
                                    0,
                                    ImPlotSpec(ImPlotProp_LineColor,
                                        col,
                                        ImPlotProp_FillColor,
                                        col,
                                        ImPlotProp_Stride,
                                        sizeof(NetworkSample)));

                                double txInv[n];
                                for (int i = 0; i < n; ++i)
                                    txInv[static_cast<std::size_t>(i)] =
                                        yMax - samples[i].txBps;
                                col = _app.GetImGui().GetColormapColor(1);
                                _app.GetImGui().PlotShaded("tx",
                                    txInv,
                                    n,
                                    yMax,
                                    1,
                                    0,
                                    ImPlotSpec(ImPlotProp_LineColor,
                                        col,
                                        ImPlotProp_FillColor,
                                        col));
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

        ImGuiIf& GetImGui() override
        {
            return _app.GetImGui();
        }

        const ImGuiIf& GetImGui() const override
        {
            return _app.GetImGui();
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

            bool showNumbers = GetShowNumbers(preferences);
            if (_app.GetImGui().Checkbox("Show numbers", &showNumbers))
                SetShowNumbers(preferences, showNumbers);

            // Y-axis maximum
            static constexpr const char* kMaximumLabels[] = {
                "Auto", "1MBps", "10Mbps", "100Mbps", "1GBps"};
            static constexpr double kMaximumValues[] = {
                0, 1'000'000, 10'000'000, 100'000'000, 1'000'000'000};
            double currentMaximum = GetMaximum(preferences);
            int maximumIndex = static_cast<int>(
                indexOf(kMaximumValues, currentMaximum).value_or(0));
            if (_app.GetImGui().SliderInt("Maximum (B/s)",
                    &maximumIndex,
                    0,
                    4,
                    kMaximumLabels[maximumIndex]))
            {
                SetMaximum(preferences, kMaximumValues[maximumIndex]);
            }

            // Visible interfaces
            auto allowedSet = GetInterfaces(preferences);

            bool changed = false;
            ImGuiStyle& style = _app.GetImGui().GetStyle();
            float window_visible_x2 =
                _app.GetImGui().GetWindowPos().x +
                _app.GetImGui().GetWindowContentRegionMax().x;

            for (size_t i = 0; i < _sensor->GetChannels(); ++i)
            {
                auto name = _sensor->GetChannelName(i);
                _app.GetImGui().PushID(static_cast<int>(i));
                bool state = allowedSet.contains(name);
                float width = _app.GetImGui().CalcTextSize(name.c_str()).x +
                              style.FramePadding.x * 2.0f;
                if (_app.GetImGui().Selectable(
                        name.c_str(), &state, 0, ImVec2(width, 0)))
                {
                    if (state)
                        allowedSet.insert(name);
                    else
                        allowedSet.erase(name);
                    changed = true;
                }
                _app.GetImGui().PopID();

                if (i + 1 < _sensor->GetChannels())
                {
                    std::string nextName = _sensor->GetChannelName(i + 1);
                    float nextWidth =
                        _app.GetImGui().CalcTextSize(nextName.c_str()).x +
                        style.FramePadding.x * 2.0f;
                    float last_x2 = _app.GetImGui().GetItemRectMax().x;
                    float next_x2 = last_x2 + style.ItemSpacing.x + nextWidth;
                    if (next_x2 < window_visible_x2)
                        _app.GetImGui().SameLine();
                }
            }

            if (changed)
                SetInterfaces(preferences, allowedSet);
        }

    private:
        double GetMaximum(const Preferences& prefs) const
        {
            return prefs.GetDouble(GetPrefName() + ".maximum").value_or(0);
        }

        void SetMaximum(Preferences& prefs, double value) const
        {
            prefs.SetDouble(GetPrefName() + ".maximum", value);
        }

        std::set<std::string> GetInterfaces(const Preferences& prefs) const
        {
            return prefs.GetStringSet(GetPrefName() + ".interfaces")
                .value_or(std::set<std::string>());
        }

        void SetInterfaces(
            Preferences& prefs, const std::set<std::string>& value) const
        {
            prefs.SetStringSet(GetPrefName() + ".interfaces", value);
        }

        bool GetShowNumbers(const Preferences& prefs) const
        {
            return prefs.GetBoolean(GetPrefName() + ".show_numbers")
                .value_or(true);
        }

        void SetShowNumbers(Preferences& prefs, bool value) const
        {
            prefs.SetBoolean(GetPrefName() + ".show_numbers", value);
        }

        App& _app;
        std::unique_ptr<NetworkSensor> _sensor;
    };

} // namespace

std::unique_ptr<View> CreateNetworkView(App& app, Sensors& sensors)
{
    return std::make_unique<NetworkViewImpl>(app, sensors);
}

std::unique_ptr<View> CreateNetworkView(
    App& app, std::unique_ptr<NetworkSensor> sensor)
{
    return std::make_unique<NetworkViewImpl>(app, std::move(sensor));
}
