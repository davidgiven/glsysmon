#include "hostname_sensor.h"

#include <unistd.h>

#include <array>
#include <memory>
#include <string>

namespace
{

    class HostnameSensorImpl : public HostnameSensor
    {
    public:
        std::string GetHostname() override
        {
            std::array<char, 256> buffer{};
            if (gethostname(buffer.data(), buffer.size()) != 0)
                return {};
            return buffer.data();
        }
    };

} // namespace

std::unique_ptr<HostnameSensor> CreateHostnameSensor()
{
    return std::make_unique<HostnameSensorImpl>();
}
