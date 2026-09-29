#include "views/view.h"

#include <imgui.h>

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "context.h"
#include "preferences/preferences.h"
#include "sensors/sensors.h"

namespace
{

    class HostnameViewImpl : public View
    {
    public:
        explicit HostnameViewImpl(const Context& ctx, Sensors& sensors):
            _context(ctx),
            _sensor(sensors.CreateHostnameSensor(GetPrefName()))
        {
        }

        explicit HostnameViewImpl(
            const Context& ctx, std::unique_ptr<HostnameSensor> sensor):
            _context(ctx),
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
        const Context& _context;
        std::unique_ptr<HostnameSensor> _sensor;
    };

} // namespace

std::unique_ptr<View> CreateHostnameView(
    const Context& ctx, Sensors& sensors)
{
    return std::make_unique<HostnameViewImpl>(ctx, sensors);
}

std::unique_ptr<View> CreateHostnameView(
    const Context& ctx, std::unique_ptr<HostnameSensor> sensor)
{
    return std::make_unique<HostnameViewImpl>(ctx, std::move(sensor));
}
