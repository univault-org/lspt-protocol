# Core library for the SRPT protocol

# Set the source files for the core library
set(CORE_SOURCES
    srpt_package.cpp
    srpt_connection.cpp
    srpt_chunking.cpp
    srpt_reassembly.cpp
    srpt_packet.cpp
    srpt_error_detection.cpp
    srpt_retransmission.cpp
    srpt_handshake.cpp
)

# Create the core library
add_library(srpt_core ${CORE_SOURCES})

# Specify include directories for the core library
target_include_directories(srpt_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link with common library
target_link_libraries(srpt_core PUBLIC srpt_common)
target_link_libraries(srpt_core PUBLIC srpt_crypto)