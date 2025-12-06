#include <cctype>
#include <stdarg.h>
#include <chrono>

#include <slogger/TimeUtils.hpp>

namespace time_utils
{
    namespace tai
    {
        std::string nanoseconds::to_TAI_timestamp() const
        {
            seconds secs(*this);

            const auto nanos_left = count() - (secs.count() * NANOS_PER_SEC);

            return std::format("{}:{}", secs.count(), nanos_left);
        }
    }

} // namespace StringUtils
