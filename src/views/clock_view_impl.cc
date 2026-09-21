#include "view.h"

#include <imgui.h>

#include <ctime>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "components.h"
#include "preferences/preferences.h"
#include "sensors/sensors.h"

namespace
{

    class ClockViewImpl : public View
    {
    public:
        explicit ClockViewImpl(const Preferences& prefs, Sensors& sensors):
            _prefs(prefs),
            _sensors(&sensors)
        {
        }

        explicit ClockViewImpl(
            const Preferences& prefs, std::unique_ptr<ClockSensor> sensor):
            _prefs(prefs),
            _sensor(std::move(sensor))
        {
        }

        void Draw() override
        {
            ClockSensor& sensor =
                _sensor ? *_sensor : _sensors->GetClockSensor();
            std::tm tm = sensor.GetLocalTime();
            char date[64];
            char time[64];
            std::strftime(date, sizeof(date), "%Y-%m-%d", &tm);
            std::strftime(time, sizeof(time), "%H:%M:%S", &tm);
            {
                const float avail = ImGui::GetContentRegionAvail().x;
                const float textWidth = ImGui::CalcTextSize(date).x;
                ImGui::SetCursorPosX(
                    ImGui::GetCursorPosX() + (avail - textWidth) * 0.5f);
                ImGui::Text("%s", date);
            }
            {
                const float avail = ImGui::GetContentRegionAvail().x;
                const float textWidth = ImGui::CalcTextSize(time).x;
                ImGui::SetCursorPosX(
                    ImGui::GetCursorPosX() + (avail - textWidth) * 0.5f);
                ImGui::Text("%s", time);
            }
        }

    private:
        const Preferences& _prefs;
        std::unique_ptr<ClockSensor> _sensor;
        Sensors* _sensors = nullptr;
    };

} // namespace

std::unique_ptr<View> CreateClockView(
    const Preferences& prefs, Sensors& sensors)
{
    return std::make_unique<ClockViewImpl>(prefs, sensors);
}

std::unique_ptr<View> CreateClockView(
    const Preferences& prefs, std::unique_ptr<ClockSensor> sensor)
{
    return std::make_unique<ClockViewImpl>(prefs, std::move(sensor));
}
