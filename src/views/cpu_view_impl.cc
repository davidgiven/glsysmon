#include "view.h"

#include <imgui.h>

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

        void Tick() override
        {
            CpuSensor& sensor = _sensor ? *_sensor : _sensors->GetCpuSensor();
            sensor.Tick();
            const std::size_t cpuCount = sensor.GetCpuCount();
            const std::size_t sampleCount = sensor.GetSampleCount();
            if (cpuCount == 0 || sampleCount == 0)
                return;
            for (std::size_t cpu = 0; cpu < cpuCount; ++cpu)
            {
                const CpuSample* samples = sensor.GetSamples(cpu);
                if (samples == nullptr)
                    continue;
                char label[32];
                std::snprintf(label, sizeof(label), "##cpu%zu", cpu);
                ImGui::PushStyleVar(
                    ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
                ImGui::PlotHistogram(label,
                    &samples[0].user,
                    static_cast<int>(sampleCount),
                    0,
                    nullptr,
                    0.0f,
                    1.0f,
                    ImVec2(ImGui::GetContentRegionAvail().x, 40),
                    sizeof(CpuSample));
                ImGui::PopStyleVar();
            }
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
