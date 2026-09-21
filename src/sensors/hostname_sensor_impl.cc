#include "hostname_sensor.h"

#include <unistd.h>

#include <array>
#include <memory>
#include <string>

#include "preferences/preferences.h"
#include "timer.h"

namespace
{

    class HostnameSensorImpl : public HostnameSensor
    {
    public:
        explicit HostnameSensorImpl(const Preferences& prefs, Timer* timer):
            _prefs(prefs),
            _timer(timer)
        {
            Tick();
        }

        void Tick() override
        {
            std::array<char, 256> buffer{};
            if (gethostname(buffer.data(), buffer.size()) != 0)
                _hostname.clear();
            else
                _hostname = buffer.data();
        }

        std::string GetHostname() override
        {
            return _hostname;
        }

    private:
        const Preferences& _prefs;
        Timer* _timer;
        std::string _hostname;
    };

} // namespace

std::unique_ptr<HostnameSensor> CreateHostnameSensor(
    const Preferences& prefs, Timer* timer)
{
    return std::make_unique<HostnameSensorImpl>(prefs, timer);
}

std::unique_ptr<HostnameSensor> CreateHostnameSensor(const Preferences& prefs)
{
    return CreateHostnameSensor(prefs, nullptr);
}
