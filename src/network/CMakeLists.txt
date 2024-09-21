add_library(lspt_network
    local/discovery.cpp
    local/routing.cpp
    hub/hub.cpp
    node/node.cpp
)

target_include_directories(lspt_network 
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${GTEST_INCLUDE_DIRS}  # Add this line
)

target_link_libraries(lspt_network
    PUBLIC
        lspt_core
        GTest::gtest  # Add this line
        GTest::gtest_main  # Add this line
)