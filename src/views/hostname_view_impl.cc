#include "views/view.h"

#include <imgui.h>

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "components.h"
#include "preferences/preferences.h"
#include "sensors/sensors.h"

namespace
{

    class HostnameViewImpl : public View
    {
    public:
        explicit HostnameViewImpl(const Preferences& prefs, Sensors& sensors):
            _prefs(prefs),
            _sensors(&sensors)
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
            HostnameSensor& sensor =
                _sensor ? *_sensor : _sensors->GetHostnameSensor();
            const std::string hostname = sensor.GetHostname();
            const float avail = ImGui::GetContentRegionAvail().x;
            const float textWidth = ImGui::CalcTextSize(hostname.c_str()).x;
            ImGui::SetCursorPosX(
                ImGui::GetCursorPosX() + (avail - textWidth) * 0.5f);
            ImGui::Text("%s", hostname.c_str());
        }

        std::string GetName() const override
        {
            return "Hostname";
        }

    private:
        const Preferences& _prefs;
        std::unique_ptr<HostnameSensor> _sensor;
        Sensors* _sensors = nullptr;
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
