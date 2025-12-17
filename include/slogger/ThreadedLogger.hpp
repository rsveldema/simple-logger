#pragma once

#include <cstdint>
#include <string>

#include "ILogger.hpp"
#include "Ring.hpp"


namespace logging
{
/** one thread can log while another can pull log entries from the log
 * This way we can delay log from a realtime task and print them in a soft-rt task.
 */
class Hard_RT_ThreadedLogger : public ILogger
{
public:
    Hard_RT_ThreadedLogger(bool debug, bool info)
    : ILogger(debug, info) {}

    void log(
        uint32_t line, const char* file, Level level, const std::string& msg) override
    {
        m_ring.add(LogEntry { line, file, level, msg });
    }

    std::optional<LogEntry> remove() override {
        return m_ring.remove();
    }
private:
    Ring<LogEntry> m_ring;
};

} // namespace logging