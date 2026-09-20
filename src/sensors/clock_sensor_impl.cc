#include "clock_sensor.h"

#include <chrono>
#include <ctime>
#include <memory>

namespace
{

    class ClockSensorImpl : public ClockSensor
    {
    public:
        std::tm GetLocalTime() override
        {
            const auto now = std::chrono::system_clock::now();
            const std::time_t time = std::chrono::system_clock::to_time_t(now);
            std::tm local{};
            if (localtime_r(&time, &local) == nullptr)
                return {};
            return local;
        }
    };

} // namespace

std::unique_ptr<ClockSensor> CreateClockSensor()
{
    return std::make_unique<ClockSensorImpl>();
}
