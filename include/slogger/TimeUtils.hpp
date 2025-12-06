#pragma once

#include <chrono>
#include <functional>
#include <cassert>

namespace time_utils
{
    /** atomic clock time since 1970-1-1
    */
    namespace tai
    {
        static constexpr auto TAI_SECS_PER_DAY = 86400ULL;
        static constexpr auto DAYS_PER_YEAR = 356ULL;
        static constexpr auto YEARS_DIFFERENCE_TO_PTP_EPOCH = 1972 - 1958;

        static constexpr auto NANOS_PER_MILLI = 1000ULL;
        static constexpr auto MILLIS_PER_SEC = 1000ULL;

        static constexpr auto MICROS_PER_SEC = 1000ULL * MILLIS_PER_SEC;
        static constexpr auto NANOS_PER_SEC = 1000ULL * MICROS_PER_SEC;

        class nanoseconds
        {
        public:
            explicit nanoseconds(uint64_t nanos) : m_nanos(nanos) {}
            uint64_t count() const { return m_nanos; }

            /** @returns TAI timestamp string: seconds:nanoseconds
             */
            std::string to_TAI_timestamp() const;
        private:
            uint64_t m_nanos;
        };

        class milliseconds
        {
        public:
            explicit milliseconds(uint64_t millis) : m_millis(millis) {}
            milliseconds(const nanoseconds& ns) : m_millis(ns.count() / NANOS_PER_MILLI) {}

            uint64_t count() const { return m_millis; }
        private:
            uint64_t m_millis;
        };

        class seconds
        {
        public:
            explicit seconds(uint64_t secs) : m_secs(secs) {}
            seconds(const milliseconds& ms) : m_secs(ms.count() / MILLIS_PER_SEC) {}
            seconds(const nanoseconds& ns) : m_secs(ns.count() / NANOS_PER_SEC) {}

            uint64_t count() const { return m_secs; }
        private:
            uint64_t m_secs;
        };


        static inline
        nanoseconds get_current_time()
        {
            // epoch in chrono::tai is 1958-01-01 00:00:00.
            // for PTP/SMPTE we need an epoch of  1970-01-01
            // THis means adding 12 years to the chono
            // For TAI a day is exactly  86,400 seconds.

            const auto tai_now = std::chrono::tai_clock::now().time_since_epoch();
            const auto secs_too_much = std::chrono::seconds(TAI_SECS_PER_DAY * DAYS_PER_YEAR * YEARS_DIFFERENCE_TO_PTP_EPOCH);
            const auto tai_adjusted = tai_now - secs_too_much;
            return nanoseconds( tai_adjusted.count() );
        }
    }

static inline
std::chrono::nanoseconds get_current_time()
{
#if 1
  struct timespec now;
  clock_gettime( CLOCK_MONOTONIC_RAW, &now );
  return std::chrono::nanoseconds(((uint64_t)now.tv_sec * UINT64_C(1000000000)) + (uint64_t)now.tv_nsec);
#else
    std::chrono::time_point<std::chrono::system_clock> now =
        std::chrono::system_clock::now();
    const auto duration = now.time_since_epoch();
    const auto nanos =
        std::chrono::duration_cast<std::chrono::nanoseconds>(duration);
    return nanos;
#endif
}

static inline
std::chrono::microseconds get_current_time_micros() {
    const auto now = get_current_time();
    return std::chrono::duration_cast<std::chrono::microseconds>(now);
}


static inline
std::chrono::milliseconds get_current_time_millis() {
    const auto now = get_current_time();
    return std::chrono::duration_cast<std::chrono::milliseconds>(now);
}

class Timeout
{
public:
    Timeout(const std::chrono::microseconds& t)
    {
        m_deadline = get_current_time() + t;
    }

    bool elapsed() const
    {
        return get_current_time() >= m_deadline;
    }

    std::chrono::nanoseconds time_left() const
    {
        return m_deadline - get_current_time();
    }

private:
    std::chrono::nanoseconds m_deadline;
};


} // namespace ptp