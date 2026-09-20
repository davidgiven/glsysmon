#include "hostname_sensor.h"

#include <unistd.h>

#include <array>
#include <memory>
#include <string>

#include "preferences/preferences.h"

namespace
{

    class HostnameSensorImpl : public HostnameSensor
    {
    public:
        explicit HostnameSensorImpl(const Preferences& prefs): _prefs(prefs) {}

        std::string GetHostname() override
        {
            std::array<char, 256> buffer{};
            if (gethostname(buffer.data(), buffer.size()) != 0)
                return {};
            return buffer.data();
        }

    private:
        const Preferences& _prefs;
    };

} // namespace

std::unique_ptr<HostnameSensor> CreateHostnameSensor(const Preferences& prefs)
{
    return std::make_unique<HostnameSensorImpl>(prefs);
}
