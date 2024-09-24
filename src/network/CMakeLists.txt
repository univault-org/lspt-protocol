add_library(srpt_network
    local/discovery.cpp
    local/routing.cpp
    hub/hub.cpp
    node/node.cpp
)

target_include_directories(srpt_network 
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${GTEST_INCLUDE_DIRS}  # Add this line
)

target_link_libraries(srpt_network
    PUBLIC
        srpt_core
        GTest::gtest  # Add this line
        GTest::gtest_main  # Add this line
)