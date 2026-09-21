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

        void Tick(Time now) override
        {
            while (!_queue.empty())
            {
                const auto it = _queue.begin();
                if (it->first > now)
                    break;
                const Time scheduled = it->first;
                Callback cb = std::move(it->second);
                _queue.erase(it);
                cb(scheduled);
            }
        }

    private:
        std::map<Time, Callback> _queue;
    };

} // namespace

std::unique_ptr<Timer> CreateTimer()
{
    return std::make_unique<TimerImpl>();
}
