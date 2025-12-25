#pragma once

#include <slogger/ITimer.hpp>

namespace time_utils
{
    class ClockRawTimer : public ITimer
    {
    public:
        std::chrono::nanoseconds get_time_ns() const override
        {
            struct timespec now;
            clock_gettime( CLOCK_MONOTONIC_RAW, &now );
            return std::chrono::nanoseconds(((uint64_t)now.tv_sec * UINT64_C(1000000000)) + (uint64_t)now.tv_nsec);
        }
    };
}