#pragma once

#include <chrono>

namespace time_utils
{
class ITimer
{
public:
    virtual ~ITimer() = default;

    virtual std::chrono::nanoseconds get_time_ns() const = 0;

    std::chrono::microseconds get_time_us() const
    {
        return std::chrono::duration_cast<std::chrono::microseconds>(
            get_time_ns());
    }

    std::chrono::milliseconds get_time_ms() const
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(
            get_time_ns());
    }

    std::chrono::seconds get_time_sec() const
    {
        return std::chrono::duration_cast<std::chrono::seconds>(get_time_ns());
    }
};

} // namespace time_utils