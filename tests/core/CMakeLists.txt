# tests/core/CMakeLists.txt
add_executable(test_srpt_core
    test_srpt_packet.cpp
    test_srpt_connection.cpp
    test_srpt_error_detection.cpp
    test_srpt_retransmission.cpp
    test_srpt_handshake.cpp
    # Add other test files as needed
)

target_link_libraries(test_srpt_core
    PRIVATE
    srpt_core
    srpt_crypto
    GTest::GTest
    GTest::Main
    ${LIBSODIUM_LIBRARY}  # Changed from ${LIBSODIUM_LIBRARIES} to ${LIBSODIUM_LIBRARY}
)

add_test(NAME test_srpt_core COMMAND test_srpt_core)

# Add this line to ensure the runtime path is set correctly
set_target_properties(test_srpt_core PROPERTIES
    BUILD_WITH_INSTALL_RPATH TRUE
    INSTALL_RPATH "${CMAKE_BINARY_DIR}/lib:${LIBSODIUM_LIBRARY_DIRS}"
)
# # tests/core/CMakeLists.txt
# add_executable(test_srpt_core
#     test_srpt_packet.cpp
#     test_srpt_connection.cpp
#     test_srpt_error_detection.cpp
#     test_srpt_retransmission.cpp
#     test_srpt_handshake.cpp
#     # Add other test files as needed
# )

# target_link_libraries(test_srpt_core
#     PRIVATE
#     srpt_core
#     srpt_crypto
#     GTest::GTest
#     GTest::Main
#     ${LIBSODIUM_LIBRARY}  # Changed from ${LIBSODIUM_LIBRARIES} to ${LIBSODIUM_LIBRARY}
# )

# add_test(NAME test_srpt_core COMMAND test_srpt_core)

# # Add this line to ensure the runtime path is set correctly
# set_target_properties(test_srpt_core PROPERTIES
#     BUILD_WITH_INSTALL_RPATH TRUE
#     INSTALL_RPATH "${CMAKE_BINARY_DIR}/lib:${LIBSODIUM_LIBRARY_DIRS}"
# )