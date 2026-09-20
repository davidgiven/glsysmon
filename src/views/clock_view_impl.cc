#include "view.h"

#include <imgui.h>

#include <ctime>
#include <memory>

#include "components.h"
#include "sensors/clock_sensor.h"

namespace
{

    class ClockViewImpl : public View
    {
    public:
        explicit ClockViewImpl(std::unique_ptr<ClockSensor> sensor):
            _sensor(std::move(sensor))
        {
        }

        void Tick() override
        {
            const std::tm tm = _sensor->GetLocalTime();
            char date[64];
            char time[64];
            std::strftime(date, sizeof(date), "%Y-%m-%d", &tm);
            std::strftime(time, sizeof(time), "%H:%M:%S", &tm);
            ImGui::Text("%s", date);
            ImGui::Text("%s", time);
        }

    private:
        std::unique_ptr<ClockSensor> _sensor;
    };

} // namespace

std::unique_ptr<View> CreateClockView()
{
    return std::make_unique<ClockViewImpl>(CreateClockSensor());
}

std::unique_ptr<View> CreateClockView(std::unique_ptr<ClockSensor> sensor)
{
    return std::make_unique<ClockViewImpl>(std::move(sensor));
}
