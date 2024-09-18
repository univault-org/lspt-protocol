cmake_minimum_required(VERSION 3.10)
project(LSPTProtocol)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find GTest package
find_package(GTest REQUIRED)

# Include directories
include_directories(${GTEST_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/src)

# Add library for LSPT core
add_library(lspt_core
    src/core/lspt_package.cpp
    src/core/lspt_chunking.cpp
    src/core/lspt_reassembly.cpp
)

# Add executable for the tests
add_executable(test_lspt_demo 
    tests/core/test_lspt_demo.cpp
    tests/core/test_lspt_package.cpp
    tests/core/test_lspt_chunking.cpp
    tests/core/test_lspt_reassembly.cpp
)

# Link the test executable against GTest and lspt_core
target_link_libraries(test_lspt_demo lspt_core ${GTEST_BOTH_LIBRARIES})

# Enable testing
enable_testing()

# Add the tests
add_test(NAME LSPTDemoTest COMMAND test_lspt_demo)

# Add examples subdirectory
add_subdirectory(examples)