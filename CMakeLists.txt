cmake_minimum_required(VERSION 3.18)

project(slogger_project CXX)

include(CTest)

enable_testing()

# Include directories
set(PUBLIC_INCLUDE_DIRECTORIES include/)
set(PRIVATE_INCLUDE_DIRECTORIES src/)

# Source files
file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")
file(GLOB_RECURSE INCLUDE_FILES "include/*.hpp")

add_library(slogger)

target_compile_features(slogger PUBLIC cxx_std_23)
target_link_libraries(slogger -luring ${UUID_LIBRARIES})
target_include_directories(slogger PUBLIC ${PUBLIC_INCLUDE_DIRECTORIES})
target_include_directories(slogger PRIVATE ${PRIVATE_INCLUDE_DIRECTORIES})

target_sources(slogger
    PUBLIC FILE_SET HEADERS
    BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include
    FILES ${INCLUDE_FILES}
    PRIVATE ${SOURCE_FILES}
)

install(TARGETS slogger FILE_SET HEADERS)

# Set target properties for public headers
set_target_properties(slogger PROPERTIES PUBLIC_HEADER "${INCLUDE_FILES}")

find_package(GTest)

if(GTest_FOUND)
add_subdirectory(tests)
endif()