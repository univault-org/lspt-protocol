cmake_minimum_required(VERSION 3.10)
project(LSPTProtocol)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find GTest package
find_package(GTest REQUIRED)

# Include directories
include_directories(${GTEST_INCLUDE_DIRS})
include_directories(src)

# Add executable for the main LSPT demo
add_executable(lspt_demo src/main.cpp)

# Add executable for the tests
add_executable(test_lspt_demo tests/core/test_lspt_demo.cpp)

# Link the test executable against GTest
target_link_libraries(test_lspt_demo ${GTEST_BOTH_LIBRARIES})

# Enable testing
enable_testing()

# Add the tests
add_test(NAME LSPTDemoTest COMMAND test_lspt_demo)add_subdirectory(examples)
add_subdirectory(examples)
