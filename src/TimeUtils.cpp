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

        
        std::optional<nanoseconds> nanoseconds::parse(const std::string& str)
        {
            // Expected format: seconds:nanoseconds
            const auto colon_pos = str.find(':');
            if (colon_pos == std::string::npos)
            {
                return std::nullopt;
            }

            const auto secs_str = str.substr(0, colon_pos);
            const auto nanos_str = str.substr(colon_pos + 1);

            try
            {
                const uint64_t secs = std::stoull(secs_str);
                const uint64_t nanos = std::stoull(nanos_str);

                return nanoseconds((secs * NANOS_PER_SEC) + nanos);
            }
            catch (const std::exception&)
            {
                return std::nullopt;
            }
        }
    }

} // namespace StringUtils
