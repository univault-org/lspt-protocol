# tests/core/CMakeLists.txt
add_executable(test_lspt_core
    test_lspt_packet.cpp
    test_lspt_connection.cpp
    test_lspt_error_detection.cpp
    test_lspt_retransmission.cpp
    test_lspt_handshake.cpp
    # Add other test files as needed
)

target_link_libraries(test_lspt_core
    PRIVATE
    lspt_core
    lspt_crypto
    GTest::GTest
    GTest::Main
    ${LIBSODIUM_LIBRARY}  # Changed from ${LIBSODIUM_LIBRARIES} to ${LIBSODIUM_LIBRARY}
)

add_test(NAME test_lspt_core COMMAND test_lspt_core)

# Add this line to ensure the runtime path is set correctly
set_target_properties(test_lspt_core PROPERTIES
    BUILD_WITH_INSTALL_RPATH TRUE
    INSTALL_RPATH "${CMAKE_BINARY_DIR}/lib:${LIBSODIUM_LIBRARY_DIRS}"
)