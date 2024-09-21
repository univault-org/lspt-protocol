# CMakeLists.txt for managing tests

# Find GTest package
find_package(GTest REQUIRED)

# Include directories for tests
include_directories(${GTEST_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/src)  # Include source directory for headers

# Set test sources
set(TEST_SOURCES
    core/test_lspt_handshake.cpp
    # space/drone/test_drone_handshake.cpp
    # ground/test_ground_handshake.cpp
    # Add other test files as needed
)


# Create the test executable
add_executable(lspt_tests ${TEST_SOURCES})

# Link libraries
target_link_libraries(lspt_tests
    PRIVATE
    lspt_core
    lspt_common
    lspt_drone_handshake  # Link the drone handshake library
    lspt_satellite_handshake  # Link the satellite handshake library
    GTest::GTest
    GTest::Main
)

# Add test to CTest
add_test(NAME lspt_tests COMMAND lspt_tests)

# Enable testing
enable_testing()

