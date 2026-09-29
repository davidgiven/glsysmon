#include "views/view.h"

#include <imgui.h>

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "context.h"
#include "imguiif.h"
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
            const float avail = _context.imgui.GetContentRegionAvail().x;
            const float textWidth = _context.imgui.CalcTextSize(hostname.c_str()).x;
            _context.imgui.SetCursorPosX(
                _context.imgui.GetCursorPosX() + (avail - textWidth) * 0.5f);
            _context.imgui.Text("%s", hostname.c_str());
        }

        std::string GetHumanName() const override
        {
            return "Hostname";
        }

        std::string GetPrefName() const override
        {
            return "hostname";
        }

        ImGuiIf& GetImGui() override
        {
            return _context.imgui;
        }

        const ImGuiIf& GetImGui() const override
        {
            return _context.imgui;
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
