add_executable(test_lspt_crypto
    test_lspt_crypto.cpp
    # Add other test files as needed
)

target_link_libraries(test_lspt_crypto
    PRIVATE
    lspt_crypto
    GTest::GTest
    GTest::Main
)

add_test(NAME test_lspt_crypto COMMAND test_lspt_crypto)