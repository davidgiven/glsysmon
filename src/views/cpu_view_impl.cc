#include "views/view.h"

#include <imgui.h>
#include <implot.h>

#include "style.h"

#include <cstdio>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "preferences/preferences.h"
#include "sensors/sensors.h"

namespace
{

    class CpuViewImpl : public View
    {
    public:
        explicit CpuViewImpl(const Preferences& prefs, Sensors& sensors):
            _prefs(prefs),
            _sensor(sensors.CreateCpuSensor())
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
            const std::size_t cpuCount = _sensor->GetChannels();
            const std::size_t sampleCount = _sensor->GetSampleCount();
            if (cpuCount == 0 || sampleCount == 0)
                return;

            Style::GraphGroup("CPU usage",
                [&]
                {
                    for (std::size_t cpu = 0; cpu < cpuCount; ++cpu)
                    {
                        const CpuSample* samples = _sensor->GetSamples(cpu);
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
                        Style::DrawGraph(std::to_string(cpu),
                            n,
                            0,
                            1,
                            [&]
                            {
                                ImPlot::PlotBarGroups(labels,
                                    values.data(),
                                    3,
                                    n,
                                    1.0,
                                    0.5,
                                    ImPlotSpec(ImPlotProp_Flags,
                                        ImPlotBarGroupsFlags_Stacked));
                            });
                    }
                });
        }

        std::string GetHumanName() const override
        {
            return "Cpu";
        }

        std::string GetPrefName() const override
        {
            return "cpu";
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
        std::unique_ptr<CpuSensor> _sensor;
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
