
# Space module CMakeLists.txt

# Set the source files for the drone handshake
set(DRONE_HANDSHAKE_SOURCES
    drone/drone_handshake.cpp  # Path to the drone handshake implementation
)

# Set the source files for the satellite handshake
set(SATELLITE_HANDSHAKE_SOURCES
    satellite/satellite_handshake.cpp  # Path to the satellite handshake implementation
)

# Create the drone handshake library
add_library(srpt_drone_handshake ${DRONE_HANDSHAKE_SOURCES})
target_include_directories(srpt_drone_handshake PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/drone)
target_link_libraries(srpt_drone_handshake PUBLIC srpt_common)

# Create the satellite handshake library
add_library(srpt_satellite_handshake ${SATELLITE_HANDSHAKE_SOURCES})
target_include_directories(srpt_satellite_handshake PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/satellite)
target_link_libraries(srpt_satellite_handshake PUBLIC srpt_common)