add_executable(test_srpt_crypto
    test_srpt_crypto.cpp
    # Add other test files as needed
)

target_link_libraries(test_srpt_crypto
    PRIVATE
    srpt_crypto
    GTest::GTest
    GTest::Main
)

add_test(NAME test_srpt_crypto COMMAND test_srpt_crypto)