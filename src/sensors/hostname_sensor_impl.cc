#include "hostname_sensor.h"

#include <imgui.h>

#include <unistd.h>

#include <array>
#include <functional>
#include <memory>
#include <string>

#include "preferences/preferences.h"
#include "timer.h"

namespace
{

    class HostnameSensorImpl : public HostnameSensor
    {
    public:
        explicit HostnameSensorImpl(const Preferences& prefs,
            Timer& timer,
            const std::string& prefPrefix):
            HostnameSensor(prefPrefix),
            _prefs(prefs),
            _timer(timer)
        {
            Tick(_timer.Now());
        }

        std::string GetHostname() override
        {
            return _hostname;
        }

        std::string GetHumanName() const override
        {
            return "Hostname";
        }

        std::string GetPrefName() const override
        {
            return "hostname";
        }

        void DrawConfiguration(Preferences& preferences) override
        {
            (void)preferences;
            ImGui::Text("%s settings", GetHumanName().c_str());
        }

    private:
        void Tick(Timer::Time t)
        {
            std::array<char, 256> buffer{};
            if (gethostname(buffer.data(), buffer.size()) != 0)
                _hostname.clear();
            else
                _hostname = buffer.data();
            _timer.Schedule(t + 10'000'000'000ULL,
                std::bind(
                    &HostnameSensorImpl::Tick, this, std::placeholders::_1));
        }

        const Preferences& _prefs;
        Timer& _timer;
        std::string _hostname;
    };

} // namespace

std::unique_ptr<HostnameSensor> CreateHostnameSensor(
    const Preferences& prefs, Timer& timer, const std::string& prefPrefix)
{
    return std::make_unique<HostnameSensorImpl>(prefs, timer, prefPrefix);
}
