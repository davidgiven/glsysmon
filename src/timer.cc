#include "timer.h"

#include <functional>
#include <map>
#include <memory>

namespace
{

    class TimerImpl : public Timer
    {
    public:
        Time Schedule(Time time, Callback callback) override
        {
            while (_queue.find(time) != _queue.end())
                ++time;
            _queue.emplace(time, std::move(callback));
            return time;
        }

        void Cancel(Time time) override
        {
            _queue.erase(time);
        }

        std::size_t Tick(Time now) override
        {
            _now = now;
            std::size_t fired = 0;
            while (!_queue.empty())
            {
                const auto it = _queue.begin();
                if (it->first > now)
                    break;
                const Time scheduled = it->first;
                Callback cb = std::move(it->second);
                _queue.erase(it);
                cb(scheduled);
                ++fired;
            }
            return fired;
        }

        Time Now() const override
        {
            return _now;
        }

        std::optional<Time> GetTimeUntilNextEvent(Time now) const override
        {
            if (_queue.empty())
                return std::nullopt;
            const Time next = _queue.begin()->first;
            if (next <= now)
                return Time{0};
            return next - now;
        }

    private:
        Time _now = 0;
        std::map<Time, Callback> _queue;
    };

} // namespace

std::unique_ptr<Timer> CreateTimer()
{
    return std::make_unique<TimerImpl>();
}
