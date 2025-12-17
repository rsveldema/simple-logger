#pragma once

/**
 * @file ILogger.hpp
 * @brief Defines the ILogger interface for logging messages with different
 * severity levels.
 *
 * This interface provides methods for logging debug, info, and error messages.
 */

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <string>

#include <format>

namespace logging
{
// Where to log to, common to impls.
enum class Level
{
    DEBUG,
    INFO,
    ERROR
};

// Where to log to, common to impls.
enum class LogOutput
{
    CONSOLE,
    FILE_STREAM
};

struct LogEntry
{
    uint32_t line;
    const char* file;
    Level level;
    std::string str;
};

class ILogger
{
public:
    ILogger(bool debug, bool info)
        : m_debug(debug)
        , m_info(info)
    {
    }

    virtual ~ILogger() {}

    void debug_msg(uint32_t line, const char* file, const std::string msg)
    {
        log(line, file, Level::DEBUG, msg);
    }

    void info_msg(uint32_t line, const char* file, const std::string msg)
    {
        log(line, file, Level::INFO, msg);
    }

    void error_msg(uint32_t line, const char* file, const std::string msg)
    {
        log(line, file, Level::ERROR, msg);
    }

    bool enable_debug() const
    {
        return m_debug;
    }

    bool enable_info() const
    {
        return m_info;
    }

    /** check if some other core/logger has something to say and print it in here
    */
    virtual void poll() {}

    virtual std::optional<LogEntry> remove() { return std::nullopt; }

    virtual void log(uint32_t line, const char* file, Level level,
        const std::string& msg) = 0;

protected:
    bool m_debug = true;
    bool m_info = true;
};

const char* strip_prefix(const char* path);


#define LOG_DEBUG(logger, ...)                                                 \
    if (logger.enable_debug())                                                 \
    {                                                                          \
        logger.debug_msg(__LINE__, logging::strip_prefix(__FILE__),            \
            std::format(__VA_ARGS__));                                         \
    }

#define LOG_INFO(logger, ...)                                                  \
    if (logger.enable_info())                                                  \
    {                                                                          \
        logger.info_msg(__LINE__, logging::strip_prefix(__FILE__),             \
            std::format(__VA_ARGS__));                                         \
    }

#define LOG_INFO_ONCE(logger, ...)                                             \
    do                                                                         \
    {                                                                          \
        static bool init_once;                                                 \
        if (logger.enable_info() && !init_once)                                \
        {                                                                      \
            logger.info_msg(__LINE__, logging::strip_prefix(__FILE__),         \
                std::format(__VA_ARGS__));                                     \
            init_once = true;                                                  \
        }                                                                      \
    } while (0)

#define LOG_ERROR(logger, ...)                                                 \
    logger.error_msg(                                                          \
        __LINE__, logging::strip_prefix(__FILE__), std::format(__VA_ARGS__))

#define LOG_ERROR_ONCE(logger, ...)                                            \
    do                                                                         \
    {                                                                          \
        static bool init_once;                                                 \
        if (!init_once)                                                        \
        {                                                                      \
            logger.error_msg(__LINE__, logging::strip_prefix(__FILE__),        \
                std::format(__VA_ARGS__));                                     \
            init_once = true;                                                  \
        }                                                                      \
    } while (0)

#define LOG_ERROR_SLOW(logger, ...)                                            \
    do                                                                         \
    {                                                                          \
        static uint32_t counter;                                               \
        if (counter++ > 1000)                                                  \
        {                                                                      \
            logger.error_msg(__LINE__, logging::strip_prefix(__FILE__),        \
                std::format(__VA_ARGS__));                                     \
            counter = 0;                                                       \
        }                                                                      \
    } while (0)

} // namespace logging