#include "view.h"

#include <fruit/fruit.h>
#include <imgui.h>

#include "components.h"
#include "sensors/hostname_sensor.h"

namespace
{

    class HostnameViewImpl : public View
    {
    public:
        using Inject = HostnameViewImpl(HostnameSensor*);

        explicit HostnameViewImpl(HostnameSensor* sensor): _sensor(sensor) {}

        void Tick() override
        {
            const std::string hostname = _sensor->GetHostname();
            ImGui::Text("%s", hostname.c_str());
        }

    private:
        HostnameSensor* _sensor;
    };

} // namespace

fruit::Component<View> GetViewComponent()
{
    return fruit::createComponent()
        .install(GetHostnameSensorComponent)
        .bind<View, HostnameViewImpl>();
}
