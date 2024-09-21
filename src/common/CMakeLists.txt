# Common library for shared types and utilities

# Set the header files for the common library
set(COMMON_HEADERS
    types.h  # Add your header files here
    # Add other header files as needed
)

# Set the source files for the common library
set(COMMON_SOURCES
    # Add any common source files here
    # If you have shared implementations, include them here
)

# Set the source files for the drone and ground handshakes
set(DRONE_HANDSHAKE_SOURCES
    ../space/drone/drone_handshake.cpp  # Path to the drone handshake implementation
)

set(GROUND_HANDSHAKE_SOURCES
    ../ground/ground_handshake.cpp  # Path to the ground handshake implementation
)

# Create the common library as a regular library
add_library(lspt_common ${COMMON_HEADERS} ${COMMON_SOURCES} ${DRONE_HANDSHAKE_SOURCES} ${GROUND_HANDSHAKE_SOURCES})

# Specify include directories for the common library
target_include_directories(lspt_common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link any necessary libraries if required (e.g., if you have dependencies)
# target_link_libraries(lspt_common ...)