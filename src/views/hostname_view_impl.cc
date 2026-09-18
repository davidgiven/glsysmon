#include "view.h"

#include <imgui.h>

#include <memory>

#include "components.h"
#include "sensors/hostname_sensor.h"

namespace
{

    class HostnameViewImpl : public View
    {
    public:
        explicit HostnameViewImpl(std::unique_ptr<HostnameSensor> sensor):
            _sensor(std::move(sensor))
        {
        }

        void Tick() override
        {
            const std::string hostname = _sensor->GetHostname();
            ImGui::Text("%s", hostname.c_str());
        }

    private:
        std::unique_ptr<HostnameSensor> _sensor;
    };

} // namespace

std::unique_ptr<View> CreateHostnameView()
{
    return std::make_unique<HostnameViewImpl>(CreateHostnameSensor());
}

std::unique_ptr<View> CreateHostnameView(std::unique_ptr<HostnameSensor> sensor)
{
    return std::make_unique<HostnameViewImpl>(std::move(sensor));
}
