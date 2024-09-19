add_executable(test_congestion_control
    test_cubic.cpp
    # Add other test files as they are created
)

target_link_libraries(test_congestion_control
    PRIVATE
    congestion_control
    GTest::GTest
    GTest::Main
)

add_test(NAME test_congestion_control COMMAND test_congestion_control)