#include "view.h"

#include <imgui.h>

#include <cstdio>
#include <memory>

#include "components.h"
#include "sensors/sensors.h"

namespace
{

    class CpuViewImpl : public View
    {
    public:
        explicit CpuViewImpl(Sensors& sensors): _sensors(&sensors) {}

        explicit CpuViewImpl(std::unique_ptr<CpuSensor> sensor):
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
                ImGui::PlotLines(label,
                    &samples[0].user,
                    static_cast<int>(sampleCount),
                    0,
                    nullptr,
                    0.0f,
                    1.0f,
                    ImVec2(ImGui::GetContentRegionAvail().x, 40),
                    sizeof(CpuSample));
            }
        }

    private:
        std::unique_ptr<CpuSensor> _sensor;
        Sensors* _sensors = nullptr;
    };

} // namespace

std::unique_ptr<View> CreateCpuView(Sensors& sensors)
{
    return std::make_unique<CpuViewImpl>(sensors);
}

std::unique_ptr<View> CreateCpuView()
{
    return CreateCpuView(Sensors::Instance());
}

std::unique_ptr<View> CreateCpuView(std::unique_ptr<CpuSensor> sensor)
{
    return std::make_unique<CpuViewImpl>(std::move(sensor));
}
