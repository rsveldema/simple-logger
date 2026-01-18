#pragma once

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <slogger/ITimer.hpp>

namespace time_utils::mocks
{
    class Timer : public time_utils::ITimer
    {
    public:
        MOCK_METHOD(std::chrono::nanoseconds, get_time_ns, (), (const, override));
    };
}