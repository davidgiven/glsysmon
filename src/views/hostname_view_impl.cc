#include "views/view.h"

#include <imgui.h>

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "preferences/preferences.h"
#include "sensors/sensors.h"

namespace
{

    class HostnameViewImpl : public View
    {
    public:
        explicit HostnameViewImpl(const Preferences& prefs, Sensors& sensors):
            _prefs(prefs),
            _sensor(sensors.CreateHostnameSensor(GetPrefName()))
        {
        }

        explicit HostnameViewImpl(
            const Preferences& prefs, std::unique_ptr<HostnameSensor> sensor):
            _prefs(prefs),
            _sensor(std::move(sensor))
        {
        }

        void Draw() override
        {
            const std::string hostname = _sensor->GetHostname();
            const float avail = ImGui::GetContentRegionAvail().x;
            const float textWidth = ImGui::CalcTextSize(hostname.c_str()).x;
            ImGui::SetCursorPosX(
                ImGui::GetCursorPosX() + (avail - textWidth) * 0.5f);
            ImGui::Text("%s", hostname.c_str());
        }

        std::string GetHumanName() const override
        {
            return "Hostname";
        }

        std::string GetPrefName() const override
        {
            return "hostname";
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
        std::unique_ptr<HostnameSensor> _sensor;
    };

} // namespace

std::unique_ptr<View> CreateHostnameView(
    const Preferences& prefs, Sensors& sensors)
{
    return std::make_unique<HostnameViewImpl>(prefs, sensors);
}

std::unique_ptr<View> CreateHostnameView(
    const Preferences& prefs, std::unique_ptr<HostnameSensor> sensor)
{
    return std::make_unique<HostnameViewImpl>(prefs, std::move(sensor));
}
