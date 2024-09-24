# Test common sources
set(TEST_COMMON_SOURCES
    test_utils.cpp
)

# Test common library
add_library(srpt_test_common ${TEST_COMMON_SOURCES})

# Include directories
target_include_directories(srpt_test_common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Linking with other libraries
target_link_libraries(srpt_test_common PUBLIC GTest::GTest srpt_common)