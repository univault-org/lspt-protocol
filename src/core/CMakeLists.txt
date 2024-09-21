# Core library for the LSPT protocol

# Set the source files for the core library
set(CORE_SOURCES
    lspt_package.cpp
    lspt_connection.cpp
    lspt_chunking.cpp
    lspt_reassembly.cpp
    lspt_packet.cpp
    lspt_error_detection.cpp
    lspt_retransmission.cpp
    lspt_handshake.cpp
)

# Create the core library
add_library(lspt_core ${CORE_SOURCES})

# Specify include directories for the core library
target_include_directories(lspt_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link with common library
target_link_libraries(lspt_core PUBLIC lspt_common)
target_link_libraries(lspt_core PUBLIC lspt_crypto)