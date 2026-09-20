#include "view.h"

#include <imgui.h>

#include <memory>

#include "components.h"
#include "sensors/sensors.h"

namespace
{

    class HostnameViewImpl : public View
    {
    public:
        explicit HostnameViewImpl(Sensors& sensors): _sensors(&sensors) {}

        explicit HostnameViewImpl(std::unique_ptr<HostnameSensor> sensor):
            _sensor(std::move(sensor))
        {
        }

        void Tick() override
        {
            HostnameSensor& sensor =
                _sensor ? *_sensor : _sensors->GetHostnameSensor();
            const std::string hostname = sensor.GetHostname();
            ImGui::Text("%s", hostname.c_str());
        }

    private:
        std::unique_ptr<HostnameSensor> _sensor;
        Sensors* _sensors = nullptr;
    };

} // namespace

std::unique_ptr<View> CreateHostnameView(Sensors& sensors)
{
    return std::make_unique<HostnameViewImpl>(sensors);
}

std::unique_ptr<View> CreateHostnameView()
{
    return CreateHostnameView(Sensors::Instance());
}

std::unique_ptr<View> CreateHostnameView(std::unique_ptr<HostnameSensor> sensor)
{
    return std::make_unique<HostnameViewImpl>(std::move(sensor));
}
