#include "views/view.h"

#include <imgui.h>

#include <ctime>
#include <memory>
#include <optional>
#include <string>
#include <vector>

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

        void Draw() override
        {
            ClockSensor& sensor = _sensors->GetClockSensor();
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

        std::string GetName() const override
        {
            return "Clock";
        }

    private:
        const Preferences& _prefs;
        Sensors* _sensors = nullptr;
    };

} // namespace

std::unique_ptr<View> CreateClockView(
    const Preferences& prefs, Sensors& sensors)
{
    return std::make_unique<ClockViewImpl>(prefs, sensors);
}
