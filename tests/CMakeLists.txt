
find_package(GTest REQUIRED)

add_executable(slogger_unittests test_stringutils.cpp test_tai.cpp)
target_include_directories(slogger_unittests PUBLIC ${GTEST_INCLUDE_DIRS})
target_link_libraries(slogger_unittests -lgtest -lgmock slogger gtest_main)


add_test(NAME googletest
         COMMAND slogger_unittests)