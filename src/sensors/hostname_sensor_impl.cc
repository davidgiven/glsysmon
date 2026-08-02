#include "hostname_sensor.h"

#include <fruit/fruit.h>
#include <unistd.h>

#include <array>
#include <string>

#include "components.h"

namespace
{

    class HostnameSensorImpl : public HostnameSensor
    {
    public:
        using Inject = HostnameSensorImpl();

        std::string GetHostname() override
        {
            std::array<char, 256> buffer{};
            if (gethostname(buffer.data(), buffer.size()) != 0)
                return {};
            return buffer.data();
        }
    };

} // namespace

fruit::Component<HostnameSensor> GetHostnameSensorComponent()
{
    return fruit::createComponent().bind<HostnameSensor, HostnameSensorImpl>();
}
