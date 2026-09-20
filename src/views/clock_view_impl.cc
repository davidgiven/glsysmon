#include "view.h"

#include <imgui.h>

#include <ctime>
#include <memory>

#include "components.h"
#include "sensors/sensors.h"

namespace
{

    class ClockViewImpl : public View
    {
    public:
        explicit ClockViewImpl(Sensors& sensors): _sensors(&sensors) {}

        explicit ClockViewImpl(std::unique_ptr<ClockSensor> sensor):
            _sensor(std::move(sensor))
        {
        }

        void Tick() override
        {
            ClockSensor& sensor =
                _sensor ? *_sensor : _sensors->GetClockSensor();
            const std::tm tm = sensor.GetLocalTime();
            char date[64];
            char time[64];
            std::strftime(date, sizeof(date), "%Y-%m-%d", &tm);
            std::strftime(time, sizeof(time), "%H:%M:%S", &tm);
            ImGui::Text("%s", date);
            ImGui::Text("%s", time);
        }

    private:
        std::unique_ptr<ClockSensor> _sensor;
        Sensors* _sensors = nullptr;
    };

} // namespace

std::unique_ptr<View> CreateClockView(Sensors& sensors)
{
    return std::make_unique<ClockViewImpl>(sensors);
}

std::unique_ptr<View> CreateClockView()
{
    return CreateClockView(Sensors::Instance());
}

std::unique_ptr<View> CreateClockView(std::unique_ptr<ClockSensor> sensor)
{
    return std::make_unique<ClockViewImpl>(std::move(sensor));
}
