add_library(congestion_control
    cubic.cpp
    # Add other source files as they are created
)

target_include_directories(congestion_control PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})