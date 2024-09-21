# Test common sources
set(TEST_COMMON_SOURCES
    test_utils.cpp
)

# Test common library
add_library(lspt_test_common ${TEST_COMMON_SOURCES})

# Include directories
target_include_directories(lspt_test_common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Linking with other libraries
target_link_libraries(lspt_test_common PUBLIC GTest::GTest lspt_common)