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

#include "context.h"
#include "imguiif.h"
#include "preferences/preferences.h"
#include "sensors/sensors.h"
#include "views/style.h"

namespace
{

    class TemperatureViewImpl : public ViewGraphMixin
    {
    public:
        explicit TemperatureViewImpl(const Context& ctx, Sensors& sensors):
            _context(ctx),
            _sensor(sensors.CreateTemperatureSensor(GetPrefName()))
        {
        }

        explicit TemperatureViewImpl(const Context& ctx, std::unique_ptr<TemperatureSensor> sensor):
            _context(ctx),
            _sensor(std::move(sensor))
        {
        }

        void Draw() override
        {
            const std::size_t count = _sensor->GetChannels();
            const std::size_t sampleCount = _sensor->GetSampleCount();
            if (count == 0 || sampleCount == 0)
                return;
            const int yMin = GetMinimum(_context.preferences);
            const int yMax = GetMaximum(_context.preferences);
            const auto allowedSet = GetSensors(_context.preferences);
            const int graphHeight = GetGraphHeight(_context.preferences);
            const bool showValue = GetShowValue(_context.preferences);

            Style::GraphGroup(_context.imgui, "Temperature",
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
                        DrawGraph(_context.imgui, _context.preferences,
                            title,
                            subtitle,
                            n,
                            yMin,
                            yMax,
                            [&]
                            {
                                _context.imgui.PlotLine(
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
            return _context.imgui;
        }

        const ImGuiIf& GetImGui() const override
        {
            return _context.imgui;
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
            float minMax[2] = {static_cast<float>(GetMinimum(preferences)),
                static_cast<float>(GetMaximum(preferences))};
            if (_context.imgui.DragFloat2("Min/Max (°C)", minMax))
            {
                SetMinimum(preferences, static_cast<int>(minMax[0]));
                SetMaximum(preferences, static_cast<int>(minMax[1]));
            }

            bool showValue = GetShowValue(preferences);
            if (_context.imgui.Checkbox("Show temperature value", &showValue))
            {
                SetShowValue(preferences, showValue);
            }

            // Visible sensors
            auto allowedSet =
                GetSensors(preferences).value_or(std::set<std::string>());
            bool changed = false;
            ImGuiStyle& style = _context.imgui.GetStyle();
            float window_visible_x2 =
                _context.imgui.GetWindowPos().x + _context.imgui.GetWindowContentRegionMax().x;
            for (size_t i = 0; i < _sensor->GetChannels(); i++)
            {
                auto name = _sensor->GetChannelName(i);
                _context.imgui.PushID(static_cast<int>(i));
                bool state = allowedSet.contains(name);
                float width = _context.imgui.CalcTextSize(name.c_str()).x +
                              style.FramePadding.x * 2.0f;
                if (_context.imgui.Selectable(name.c_str(), state, 0, ImVec2(width, 0)))
                {
                    if (state)
                        allowedSet.erase(name);
                    else
                        allowedSet.insert(name);
                    changed = true;
                }
                _context.imgui.PopID();
                if (i + 1 < _sensor->GetChannels())
                {
                    std::string nextName = _sensor->GetChannelName(i + 1);
                    float nextWidth = _context.imgui.CalcTextSize(nextName.c_str()).x +
                                      style.FramePadding.x * 2.0f;
                    float last_x2 = _context.imgui.GetItemRectMax().x;
                    float next_x2 = last_x2 + style.ItemSpacing.x + nextWidth;
                    if (next_x2 < window_visible_x2)
                        _context.imgui.SameLine();
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

        const Context& _context;
        std::unique_ptr<TemperatureSensor> _sensor;
    };

} // namespace

std::unique_ptr<View> CreateTemperatureView(const Context& ctx, Sensors& sensors)
{
    return std::make_unique<TemperatureViewImpl>(ctx, sensors);
}

std::unique_ptr<View> CreateTemperatureView(const Context& ctx, std::unique_ptr<TemperatureSensor> sensor)
{
    return std::make_unique<TemperatureViewImpl>(ctx, std::move(sensor));
}
