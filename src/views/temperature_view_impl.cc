#include "app.h"
#include "views/view.h"
#include "views/view_graph_mixin.h"

#include <imgui.h>
#include <implot.h>

#include <cmath>
#include <cstdio>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <vector>

#include "imguiif.h"
#include "preferences/preferences.h"
#include "sensors/sensors.h"
#include "views/style.h"

namespace
{

    class TemperatureViewImpl : public ViewGraphMixin
    {
    public:
        explicit TemperatureViewImpl(App& app, Sensors& sensors):
            _app(app),
            _sensor(sensors.CreateTemperatureSensor(GetPrefName()))
        {
        }

        explicit TemperatureViewImpl(
            App& app, std::unique_ptr<TemperatureSensor> sensor):
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
            const int yMin = GetMinimum(_app.GetPreferences());
            const int yMax = GetMaximum(_app.GetPreferences());
            const auto allowedSet = GetSensors(_app.GetPreferences());
            const int graphHeight = GetGraphHeight(_app.GetPreferences());
            const bool showValue = GetShowValue(_app.GetPreferences());

            Style::GraphGroup(_app.GetImGui(),
                "Temperature",
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
                        std::string title = channelName;
                        std::string subtitle;
                        if (showValue)
                            subtitle =
                                std::to_string(static_cast<long long>(
                                    std::llround(samples[sampleCount - 1]))) +
                                "°C";
                        DrawGraph(
                            _app.GetImGui(),
                            _app.GetPreferences(),
                            title,
                            subtitle,
                            n,
                            yMin,
                            yMax,
                            [&]
                            {
                                _app.GetImGui().PlotLine(
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

            // Temperature range
            int minimum = GetMinimum(preferences);
            if (_app.GetImGui().InputInt("Minimum (°C)", &minimum))
            {
                SetMinimum(preferences, minimum);
            }
            int maximum = GetMaximum(preferences);
            if (_app.GetImGui().InputInt("Maximum (°C)", &maximum))
            {
                SetMaximum(preferences, maximum);
            }

            bool showValue = GetShowValue(preferences);
            if (_app.GetImGui().Checkbox("Show temperature value", &showValue))
            {
                SetShowValue(preferences, showValue);
            }

            // Visible sensors
            auto allowedSet =
                GetSensors(preferences).value_or(std::set<std::string>());
            bool changed = false;
            ImGuiStyle& style = _app.GetImGui().GetStyle();
            float window_visible_x2 =
                _app.GetImGui().GetWindowPos().x +
                _app.GetImGui().GetWindowContentRegionMax().x;
            for (size_t i = 0; i < _sensor->GetChannels(); i++)
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
                SetSensors(preferences, allowedSet);
        }

    private:
        int GetMinimum(const Preferences& prefs) const
        {
            return prefs.GetInteger(GetPrefName() + ".minimum").value_or(20);
        }

        void SetMinimum(Preferences& prefs, int value) const
        {
            prefs.SetInteger(GetPrefName() + ".minimum", value);
        }

        int GetMaximum(const Preferences& prefs) const
        {
            return prefs.GetInteger(GetPrefName() + ".maximum").value_or(80);
        }

        void SetMaximum(Preferences& prefs, int value) const
        {
            prefs.SetInteger(GetPrefName() + ".maximum", value);
        }

        std::optional<std::set<std::string>> GetSensors(
            const Preferences& prefs) const
        {
            return prefs.GetStringSet(GetPrefName() + ".sensors");
        }

        void SetSensors(
            Preferences& prefs, const std::set<std::string>& value) const
        {
            prefs.SetStringSet(GetPrefName() + ".sensors", value);
        }

        bool GetShowValue(const Preferences& prefs) const
        {
            return prefs.GetBoolean(GetPrefName() + ".show_value")
                .value_or(true);
        }

        void SetShowValue(Preferences& prefs, bool value) const
        {
            prefs.SetBoolean(GetPrefName() + ".show_value", value);
        }

        App& _app;
        std::unique_ptr<TemperatureSensor> _sensor;
    };

} // namespace

std::unique_ptr<View> CreateTemperatureView(
    App& app, Sensors& sensors)
{
    return std::make_unique<TemperatureViewImpl>(app, sensors);
}

std::unique_ptr<View> CreateTemperatureView(
    App& app, std::unique_ptr<TemperatureSensor> sensor)
{
    return std::make_unique<TemperatureViewImpl>(app, std::move(sensor));
}
