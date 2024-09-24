find_package(GTest REQUIRED)

# Include directories for integration tests
include_directories(${GTEST_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/include)  # Include the main include directory
include_directories(${PROJECT_SOURCE_DIR}/src)  # Include source directory for headers

# Set integration test sources
set(SATELLITE_TEST_SOURCES
    test_satellite_api.cpp
    test_iridium_mock_api.cpp
    mocks/iridium_mock_api.cpp
    # Add other integration test files as needed
)

# Create the integration test executable
add_executable(lspt_satellite_tests ${SATELLITE_TEST_SOURCES})

# Link libraries
target_link_libraries(lspt_satellite_tests
    PRIVATE
    GTest::GTest
    GTest::Main
    lspt_satellite
    # Add any other dependencies here
)

# Add include directories using target_include_directories
target_include_directories(lspt_satellite_tests
    PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/include
    ${GTEST_INCLUDE_DIRS}
)

# Enable testing
enable_testing()

# Add integration test to CTest
add_test(NAME lspt_satellite_tests COMMAND lspt_satellite_tests)