# CMakeLists.txt for managing integration tests

# Find GTest package
find_package(GTest REQUIRED)

# Include directories for integration tests
include_directories(${GTEST_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/src)  # Include source directory for headers

# Set integration test sources
set(INTEGRATION_TEST_SOURCES
    test_handshake_integration.cpp
    # Add other integration test files as needed
)

# Create the integration test executable
add_executable(lspt_integration_tests ${INTEGRATION_TEST_SOURCES})

# Link libraries
target_link_libraries(lspt_integration_tests
    PRIVATE
    lspt_core
    lspt_common
    GTest::GTest
    GTest::Main
)

# Enable testing
enable_testing()

# Add integration test to CTest
add_test(NAME lspt_integration_tests COMMAND lspt_integration_tests)