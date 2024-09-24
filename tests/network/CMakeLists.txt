add_executable(network_tests
    local/discovery_test.cpp
    local/routing_test.cpp
    hub/hub_test.cpp
    node/node_test.cpp
)

target_link_libraries(network_tests
    PRIVATE
    GTest::GTest
    GTest::Main
    srpt_network
    # Add any other dependencies here
)

target_include_directories(network_tests
PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${GTEST_INCLUDE_DIRS}
)

add_test(NAME network_tests COMMAND network_tests)