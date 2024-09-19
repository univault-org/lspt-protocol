# tests/core/CMakeLists.txt
add_executable(test_lspt_core
    test_lspt_packet.cpp
    test_lspt_error_detection.cpp
    test_lspt_retransmission.cpp
    # Add other test files as needed
)

target_link_libraries(test_lspt_core
    PRIVATE
    lspt_core
    GTest::GTest
    GTest::Main
)

add_test(NAME test_lspt_core COMMAND test_lspt_core)