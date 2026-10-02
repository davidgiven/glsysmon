#include "app.h"
#include "views/view.h"

#include <imgui.h>

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "imguiif.h"
#include "preferences/preferences.h"
#include "sensors/sensors.h"

namespace
{

    class HostnameViewImpl : public View
    {
    public:
        explicit HostnameViewImpl(App& app, Sensors& sensors):
            _app(app),
            _sensor(sensors.CreateHostnameSensor(GetPrefName()))
        {
        }

        explicit HostnameViewImpl(
            App& app, std::unique_ptr<HostnameSensor> sensor):
            _app(app),
            _sensor(std::move(sensor))
        {
        }

        void Draw() override
        {
            const std::string hostname = _sensor->GetHostname();
            const float avail = _app.GetImGui().GetContentRegionAvail().x;
            const float textWidth = _app.GetImGui().CalcTextSize(hostname.c_str()).x;
            _app.GetImGui().SetCursorPosX(
                _app.GetImGui().GetCursorPosX() + (avail - textWidth) * 0.5f);
            _app.GetImGui().Text("%s", hostname.c_str());
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
            return _app.GetImGui();
        }

        const ImGuiIf& GetImGui() const override
        {
            return _app.GetImGui();
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
        App& _app;
        std::unique_ptr<HostnameSensor> _sensor;
    };

} // namespace

std::unique_ptr<View> CreateHostnameView(
    App& app, Sensors& sensors)
{
    return std::make_unique<HostnameViewImpl>(app, sensors);
}

std::unique_ptr<View> CreateHostnameView(
    App& app, std::unique_ptr<HostnameSensor> sensor)
{
    return std::make_unique<HostnameViewImpl>(app, std::move(sensor));
}
