#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "ILogger.hpp"


namespace logging
{

class DirectConsoleLogger : public ILogger
{
public:
    DirectConsoleLogger(bool debug, bool info, LogOutput output);
    ~DirectConsoleLogger();

    void log(uint32_t line, const char* file, Level level,
        const std::string& msg) override;

    /** check if some other core has something to say and print it in here
    */
    void poll() override;

    void add(const std::shared_ptr<ILogger>& logger)
    {
        m_poll_loggers.push_back(logger);
    }

private:
    LogOutput m_output;
    FILE* m_f = nullptr;

    std::vector<std::shared_ptr<ILogger> > m_poll_loggers;
};

} // namespace logging