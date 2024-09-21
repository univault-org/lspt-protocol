# Collect all the test source files in this directory
set(AI_DATA_TEST_SOURCES
    vectorized_data_test.cpp
)

# Create an executable target for the AI data tests
add_executable(ai_data_tests ${AI_DATA_TEST_SOURCES})

# Link against the ai_data library and Google Test
target_link_libraries(ai_data_tests
    PRIVATE
    ai_data
    gtest
    gtest_main
)

# Add this test to CTest
add_test(NAME ai_data_tests COMMAND ai_data_tests)

# Set C++ standard for this target
target_compile_features(ai_data_tests PRIVATE cxx_std_17)