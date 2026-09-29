#include "views/view.h"
#include "views/view_graph_mixin.h"

#include <imgui.h>
#include <implot.h>

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "context.h"
#include "imguiif.h"
#include "preferences/preferences.h"
#include "sensors/sensors.h"
#include "utils.h"
#include "views/style.h"

namespace
{

    class MemoryViewImpl : public ViewGraphMixin
    {
    public:
        explicit MemoryViewImpl(const Context& ctx, Sensors& sensors):
            _context(ctx),
            _sensor(sensors.CreateMemorySensor(GetPrefName()))
        {
        }

        explicit MemoryViewImpl(const Context& ctx, std::unique_ptr<MemorySensor> sensor):
            _context(ctx),
            _sensor(std::move(sensor))
        {
        }

        void Draw() override
        {
            const std::size_t sampleCount = _sensor->GetSampleCount();
            if (sampleCount == 0)
                return;
            if (_sensor->GetChannels() == 0)
                return;

            const int graphHeight = GetGraphHeight(_context.preferences);

            Style::GraphGroup(_context.imgui, "Memory",
                [&]
                {
                    const MemorySample* samples = _sensor->GetSamples(0);
                    if (samples == nullptr)
                        return;
                    const int n = static_cast<int>(sampleCount);

                    std::vector<double> values;
                    values.reserve(static_cast<std::size_t>(n));
                    double maxTotal = 0;
                    for (int i = 0; i < n; ++i)
                    {
                        values.push_back(
                            static_cast<double>(samples[i].usedRam));
                        double t = static_cast<double>(samples[i].totalRam);
                        if (t > maxTotal)
                            maxTotal = t;
                    }
                    double yMax = maxTotal > 0 ? maxTotal * 1.1 : 1;
                    if (maxTotal <= 0)
                    {
                        double maxVal = 0;
                        for (double v : values)
                            maxVal = std::max(maxVal, v);
                        if (maxVal > 0)
                            yMax = maxVal * 1.1;
                        else
                            yMax = 1;
                    }

                    const MemorySample& last = samples[sampleCount - 1];
                    DrawGraph(_context.imgui, _context.preferences,
                        "",
                        std::to_string(std::llround(
                            100.0 * last.usedRam / last.totalRam)) +
                            "%",
                        n,
                        0,
                        yMax,
                        [&]
                        {
                            _context.imgui.PlotLine("used", values.data(), n);
                        },
                        static_cast<float>(graphHeight));
                });
        }

        std::string GetHumanName() const override
        {
            return "Memory";
        }

        std::string GetPrefName() const override
        {
            return "memory";
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

    private:
        const Context& _context;
        std::unique_ptr<MemorySensor> _sensor;
    };

} // namespace

std::unique_ptr<View> CreateMemoryView(const Context& ctx, Sensors& sensors)
{
    return std::make_unique<MemoryViewImpl>(ctx, sensors);
}

std::unique_ptr<View> CreateMemoryView(const Context& ctx, std::unique_ptr<MemorySensor> sensor)
{
    return std::make_unique<MemoryViewImpl>(ctx, std::move(sensor));
}
