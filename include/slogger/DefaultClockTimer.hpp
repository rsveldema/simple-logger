#pragma once

#include <slogger/ITimer.hpp>

namespace time_utils
{
    class DefaultClockTimer : public ITimer
    {
    public:
        std::chrono::nanoseconds get_time_ns() const override
        {
            return std::chrono::duration_cast<std::chrono::nanoseconds>(
                std::chrono::steady_clock::now().time_since_epoch());
        }
    };
}