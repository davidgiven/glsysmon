#include "clock_sensor.h"

#include <imgui.h>

#include <chrono>
#include <ctime>
#include <functional>
#include <memory>
#include <string>

#include "context.h"
#include "preferences/preferences.h"
#include "timer.h"

namespace
{

    class ClockSensorImpl : public ClockSensor
    {
    public:
        explicit ClockSensorImpl(
            const Context& ctx, const std::string& prefPrefix): ClockSensor(ctx, prefPrefix),
            _ctx(ctx)
        {
            Tick(_ctx.timer.Now());
        }

        std::tm GetLocalTime() override
        {
            return _tm;
        }

        std::string GetHumanName() const override
        {
            return "Clock";
        }

        std::string GetPrefName() const override
        {
            return "clock";
        }

    private:
        void Tick(Timer::Time t)
        {
            const auto now = std::chrono::system_clock::now();
            const std::time_t time = std::chrono::system_clock::to_time_t(now);
            std::tm local{};
            if (localtime_r(&time, &local) == nullptr)
                _tm = {};
            else
                _tm = local;
            _ctx.timer.Schedule(t + 1'000'000'000ULL,
                std::bind(&ClockSensorImpl::Tick, this, std::placeholders::_1));
        }

        const Context& _ctx;
        std::tm _tm{};
    };

} // namespace

std::unique_ptr<ClockSensor> CreateClockSensor(
    const Context& ctx, const std::string& prefPrefix)
{
    return std::make_unique<ClockSensorImpl>(ctx, prefPrefix);
}
