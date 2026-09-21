#include "clock_sensor.h"

#include <chrono>
#include <ctime>
#include <memory>

#include "preferences/preferences.h"

namespace
{

    class ClockSensorImpl : public ClockSensor
    {
    public:
        explicit ClockSensorImpl(const Preferences& prefs): _prefs(prefs)
        {
            Tick();
        }

        void Tick() override
        {
            const auto now = std::chrono::system_clock::now();
            const std::time_t time = std::chrono::system_clock::to_time_t(now);
            std::tm local{};
            if (localtime_r(&time, &local) == nullptr)
                _tm = {};
            else
                _tm = local;
        }

        std::tm GetLocalTime() override
        {
            return _tm;
        }

    private:
        const Preferences& _prefs;
        std::tm _tm{};
    };

} // namespace

std::unique_ptr<ClockSensor> CreateClockSensor(const Preferences& prefs)
{
    return std::make_unique<ClockSensorImpl>(prefs);
}
