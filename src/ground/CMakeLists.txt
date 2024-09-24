# Ground module CMakeLists.txt

# Set the source files for the ground handshake
set(GROUND_HANDSHAKE_SOURCES
    ground_handshake.cpp  # Path to the ground handshake implementation
)

# Create the ground handshake library
add_library(srpt_ground_handshake ${GROUND_HANDSHAKE_SOURCES})

# Specify include directories for the ground handshake
target_include_directories(srpt_ground_handshake PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link with the common library
target_link_libraries(srpt_ground_handshake PUBLIC srpt_common)