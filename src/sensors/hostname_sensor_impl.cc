#include "hostname_sensor.h"

#include <imgui.h>

#include <unistd.h>

#include <array>
#include <functional>
#include <memory>
#include <string>

#include "context.h"
#include "preferences/preferences.h"
#include "timer.h"

namespace
{

    class HostnameSensorImpl : public HostnameSensor
    {
    public:
        explicit HostnameSensorImpl(
            const Context& ctx, const std::string& prefPrefix): HostnameSensor(ctx, prefPrefix),
            _ctx(ctx)
        {
            Tick(_ctx.timer.Now());
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

    private:
        void Tick(Timer::Time t)
        {
            std::array<char, 256> buffer{};
            if (gethostname(buffer.data(), buffer.size()) != 0)
                _hostname.clear();
            else
                _hostname = buffer.data();
            _ctx.timer.Schedule(t + 10'000'000'000ULL,
                std::bind(
                    &HostnameSensorImpl::Tick, this, std::placeholders::_1));
        }

        const Context& _ctx;
        std::string _hostname;
    };

} // namespace

std::unique_ptr<HostnameSensor> CreateHostnameSensor(
    const Context& ctx, const std::string& prefPrefix)
{
    return std::make_unique<HostnameSensorImpl>(ctx, prefPrefix);
}
