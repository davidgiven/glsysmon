#include "clock_sensor.h"

#include <chrono>
#include <ctime>
#include <functional>
#include <memory>
#include <string>

#include "preferences/preferences.h"
#include "timer.h"

namespace
{

    class ClockSensorImpl : public ClockSensor
    {
    public:
        explicit ClockSensorImpl(const Preferences& prefs, Timer& timer):
            _prefs(prefs),
            _timer(timer)
        {
            Tick(_timer.Now());
        }

        std::tm GetLocalTime() override
        {
            return _tm;
        }

        std::string GetName() const override
        {
            return "Clock";
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
            _timer.Schedule(t + 1'000'000'000ULL,
                std::bind(&ClockSensorImpl::Tick, this, std::placeholders::_1));
        }

        const Preferences& _prefs;
        Timer& _timer;
        std::tm _tm{};
    };

} // namespace

std::unique_ptr<ClockSensor> CreateClockSensor(
    const Preferences& prefs, Timer& timer)
{
    return std::make_unique<ClockSensorImpl>(prefs, timer);
}
