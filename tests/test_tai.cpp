#include <gtest/gtest.h>

#include <slogger/TimeUtils.hpp>
#include <thread>
#include <chrono>

namespace Tests
{

TEST(TestTAI, get_current_time_returns_valid_time)
{
    // Test that get_current_time() returns a valid time with both seconds and nanoseconds
    auto current_time = time_utils::tai::get_current_time();
    
    // TAI time should have a reasonable epoch (should be after 1970)
    // Seconds should be positive and reasonably large
    ASSERT_GT(current_time.get_secs(), 0ULL);
    
    // Nanoseconds should be less than 1 second (1 billion nanoseconds)
    ASSERT_LT(current_time.get_nanos(), time_utils::tai::NANOS_PER_SEC);
}

TEST(TestTAI, get_current_time_advances)
{
    // Test that consecutive calls to get_current_time() advance in time
    auto time1 = time_utils::tai::get_current_time();
    
    // Sleep for a small amount to ensure time advances
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    
    auto time2 = time_utils::tai::get_current_time();
    
    // time2 should be greater than time1
    ASSERT_TRUE(time2 > time1);
}

TEST(TestTAI, nanoseconds_construction)
{
    // Test construction with seconds and nanoseconds
    time_utils::tai::nanoseconds ns(100, 500000000);
    
    ASSERT_EQ(ns.get_secs(), 100ULL);
    ASSERT_EQ(ns.get_nanos(), 500000000ULL);
}

TEST(TestTAI, nanoseconds_copy_constructor)
{
    // Test copy constructor
    time_utils::tai::nanoseconds ns1(100, 500000000);
    time_utils::tai::nanoseconds ns2(ns1);
    
    ASSERT_EQ(ns2.get_secs(), 100ULL);
    ASSERT_EQ(ns2.get_nanos(), 500000000ULL);
}

TEST(TestTAI, nanoseconds_copy_assignment)
{
    // Test copy assignment
    time_utils::tai::nanoseconds ns1(100, 500000000);
    time_utils::tai::nanoseconds ns2;
    ns2 = ns1;
    
    ASSERT_EQ(ns2.get_secs(), 100ULL);
    ASSERT_EQ(ns2.get_nanos(), 500000000ULL);
}

TEST(TestTAI, nanoseconds_addition_without_overflow)
{
    // Test addition without nanosecond overflow
    time_utils::tai::nanoseconds ns1(10, 100000000); // 10.1 seconds
    time_utils::tai::nanoseconds ns2(5, 200000000);  // 5.2 seconds
    
    auto result = ns1 + ns2;
    
    ASSERT_EQ(result.get_secs(), 15ULL);
    ASSERT_EQ(result.get_nanos(), 300000000ULL); // 0.3 seconds
}

TEST(TestTAI, nanoseconds_addition_with_overflow)
{
    // Test addition with nanosecond overflow (nanos > 1 second)
    time_utils::tai::nanoseconds ns1(10, 600000000); // 10.6 seconds
    time_utils::tai::nanoseconds ns2(5, 500000000);  // 5.5 seconds
    
    auto result = ns1 + ns2;
    
    // Should be 16 seconds and 100000000 nanoseconds (16.1 seconds total)
    ASSERT_EQ(result.get_secs(), 16ULL);
    ASSERT_EQ(result.get_nanos(), 100000000ULL);
}

TEST(TestTAI, nanoseconds_greater_than_operator)
{
    // Test > operator
    time_utils::tai::nanoseconds ns1(10, 500000000);
    time_utils::tai::nanoseconds ns2(10, 400000000);
    time_utils::tai::nanoseconds ns3(9, 900000000);
    time_utils::tai::nanoseconds ns4(10, 500000000);
    
    ASSERT_TRUE(ns1 > ns2);  // Same seconds, ns1 has more nanos
    ASSERT_TRUE(ns1 > ns3);  // ns1 has more seconds
    ASSERT_FALSE(ns1 > ns4); // Equal
    ASSERT_FALSE(ns2 > ns1); // ns2 is smaller
}

TEST(TestTAI, nanoseconds_greater_equal_operator)
{
    // Test >= operator
    time_utils::tai::nanoseconds ns1(10, 500000000);
    time_utils::tai::nanoseconds ns2(10, 400000000);
    time_utils::tai::nanoseconds ns3(10, 500000000);
    time_utils::tai::nanoseconds ns4(11, 100000000);
    
    ASSERT_TRUE(ns1 >= ns2);  // ns1 is greater
    ASSERT_TRUE(ns1 >= ns3);  // Equal
    ASSERT_TRUE(ns4 >= ns1);  // ns4 is greater
    ASSERT_FALSE(ns2 >= ns1); // ns2 is smaller
}

TEST(TestTAI, nanoseconds_to_TAI_timestamp)
{
    // Test to_TAI_timestamp() conversion
    time_utils::tai::nanoseconds ns(1234567890, 123456789);
    
    std::string timestamp = ns.to_TAI_timestamp();
    
    ASSERT_EQ(timestamp, "1234567890:123456789");
}

TEST(TestTAI, nanoseconds_parse_valid)
{
    // Test parsing a valid TAI timestamp
    std::string timestamp = "1234567890:123456789";
    
    auto parsed = time_utils::tai::nanoseconds::parse(timestamp);
    
    ASSERT_TRUE(parsed.has_value());
    ASSERT_EQ(parsed->get_secs(), 1234567890ULL);
    ASSERT_EQ(parsed->get_nanos(), 123456789ULL);
}

TEST(TestTAI, nanoseconds_parse_invalid_no_colon)
{
    // Test parsing an invalid timestamp (no colon)
    std::string timestamp = "1234567890";
    
    auto parsed = time_utils::tai::nanoseconds::parse(timestamp);
    
    ASSERT_FALSE(parsed.has_value());
}

TEST(TestTAI, nanoseconds_parse_invalid_format)
{
    // Test parsing an invalid timestamp (non-numeric)
    std::string timestamp = "abc:def";
    
    auto parsed = time_utils::tai::nanoseconds::parse(timestamp);
    
    ASSERT_FALSE(parsed.has_value());
}

TEST(TestTAI, nanoseconds_parse_roundtrip)
{
    // Test that to_TAI_timestamp() and parse() are inverses
    time_utils::tai::nanoseconds original(9876543210, 987654321);
    
    std::string timestamp = original.to_TAI_timestamp();
    auto parsed = time_utils::tai::nanoseconds::parse(timestamp);
    
    ASSERT_TRUE(parsed.has_value());
    ASSERT_EQ(parsed->get_secs(), original.get_secs());
    ASSERT_EQ(parsed->get_nanos(), original.get_nanos());
}

TEST(TestTAI, milliseconds_from_value)
{
    // Test milliseconds construction from value
    time_utils::tai::milliseconds ms(5000);
    
    ASSERT_EQ(ms.count(), 5000ULL);
}

TEST(TestTAI, milliseconds_from_nanoseconds)
{
    // Test milliseconds conversion from nanoseconds
    // 10 seconds and 500 million nanoseconds = 10,500 milliseconds
    time_utils::tai::nanoseconds ns(10, 500000000);
    time_utils::tai::milliseconds ms(ns);
    
    ASSERT_EQ(ms.count(), 10500ULL);
}

TEST(TestTAI, milliseconds_from_nanoseconds_precision)
{
    // Test that nanoseconds below millisecond precision are truncated
    // 10 seconds and 500,500,000 nanoseconds = 10,500 milliseconds (500,000 nanos truncated)
    time_utils::tai::nanoseconds ns(10, 500500000);
    time_utils::tai::milliseconds ms(ns);
    
    ASSERT_EQ(ms.count(), 10500ULL);
}

TEST(TestTAI, seconds_from_value)
{
    // Test seconds construction from value
    time_utils::tai::seconds sec(100);
    
    ASSERT_EQ(sec.count(), 100ULL);
}

TEST(TestTAI, seconds_from_milliseconds)
{
    // Test seconds conversion from milliseconds
    // 15,000 milliseconds = 15 seconds
    time_utils::tai::milliseconds ms(15000);
    time_utils::tai::seconds sec(ms);
    
    ASSERT_EQ(sec.count(), 15ULL);
}

TEST(TestTAI, seconds_from_milliseconds_truncation)
{
    // Test that milliseconds are truncated when converting to seconds
    // 15,500 milliseconds = 15 seconds (500 ms truncated)
    time_utils::tai::milliseconds ms(15500);
    time_utils::tai::seconds sec(ms);
    
    ASSERT_EQ(sec.count(), 15ULL);
}

TEST(TestTAI, seconds_from_nanoseconds)
{
    // Test seconds conversion from nanoseconds
    // 25 seconds and 0 nanoseconds = 25 seconds
    time_utils::tai::nanoseconds ns(25, 0);
    time_utils::tai::seconds sec(ns);
    
    ASSERT_EQ(sec.count(), 25ULL);
}

TEST(TestTAI, seconds_from_nanoseconds_truncation)
{
    // Test that nanoseconds are truncated when converting to seconds
    // 25 seconds and 999,999,999 nanoseconds = 25 seconds
    time_utils::tai::nanoseconds ns(25, 999999999);
    time_utils::tai::seconds sec(ns);
    
    ASSERT_EQ(sec.count(), 25ULL);
}

TEST(TestTAI, time_conversions_chain)
{
    // Test conversion chain: nanoseconds -> milliseconds -> seconds
    time_utils::tai::nanoseconds ns(100, 750000000);  // 100.75 seconds
    time_utils::tai::milliseconds ms(ns);              // Should be 100750 ms
    time_utils::tai::seconds sec(ms);                  // Should be 100 seconds
    
    ASSERT_EQ(ms.count(), 100750ULL);
    ASSERT_EQ(sec.count(), 100ULL);
}

TEST(TestTAI, get_current_time_produces_convertible_values)
{
    // Test that get_current_time() produces values that can be converted
    auto current_ns = time_utils::tai::get_current_time();
    time_utils::tai::milliseconds current_ms(current_ns);
    time_utils::tai::seconds current_sec(current_ns);
    
    // Verify conversions are reasonable
    ASSERT_GT(current_ms.count(), 0ULL);
    ASSERT_GT(current_sec.count(), 0ULL);
    
    // Verify relationship between conversions
    // milliseconds should be >= seconds * 1000
    ASSERT_GE(current_ms.count(), current_sec.count() * 1000);
}

} // namespace Tests
