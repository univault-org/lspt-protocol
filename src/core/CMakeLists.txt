add_library(lspt_core
    lspt_package.cpp
    lspt_connection.cpp
    lspt_chunking.cpp
    lspt_reassembly.cpp
    lspt_packet.cpp
    lspt_connection.cpp
    lspt_error_detection.cpp
    lspt_retransmission.cpp
    lspt_handshake.cpp
)
target_link_libraries(lspt_core PUBLIC lspt_crypto)
target_include_directories(lspt_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})