cmake_minimum_required(VERSION 3.10)
project(LSPTProtocol)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find OpenSSL
find_package(OpenSSL REQUIRED)

# Find GTest
find_package(GTest REQUIRED)

# Include directories
include_directories(${GTEST_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/src)

# Add subdirectories
add_subdirectory(src/core)
add_subdirectory(src/crypto)
add_subdirectory(tests/core)
add_subdirectory(tests/crypto)

# Enable testing
enable_testing()


# cmake_minimum_required(VERSION 3.10)
# project(LSPTProtocol)

# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)


# # Find OpenSSL
# find_package(OpenSSL REQUIRED)
# # Find GTest package
# find_package(GTest REQUIRED)

# # Include directories
# include_directories(${GTEST_INCLUDE_DIRS})
# include_directories(${PROJECT_SOURCE_DIR}/src)

# # Add subdirectories
# add_subdirectory(src/core)
# add_subdirectory(src/crypto)
# add_subdirectory(tests/core)
# add_subdirectory(tests/crypto)

# # Enable testing
# enable_testing()

# # Add library for LSPT core
# add_library(lspt_core
#     src/core/lspt_package.cpp
#     src/core/lspt_chunking.cpp
#     src/core/lspt_reassembly.cpp
#     src/core/lspt_packet.cpp
#     src/core/lspt_error_detection.cpp
#     src/core/lspt_retransmission.cpp
# )
# target_include_directories(lspt_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# add_library(lspt_crypto
#     src/crypto/lspt_crypto.cpp
# )

# target_include_directories(lspt_crypto PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
# target_link_libraries(lspt_crypto PUBLIC OpenSSL::SSL OpenSSL::Crypto)

# # Add executable for the tests
# add_executable(test_lspt_core 
#     tests/core/test_lspt_package.cpp
#     tests/core/test_lspt_chunking.cpp
#     tests/core/test_lspt_reassembly.cpp
#     tests/core/test_lspt_packet.cpp
#     tests/core/test_lspt_error_detection.cpp
#     tests/core/test_lspt_retransmission.cpp
# )

# target_link_libraries(test_lspt_core
#     PRIVATE
#     lspt_core
#     GTest::GTest
#     GTest::Main
# )

# add_test(NAME test_lspt_core COMMAND test_lspt_core)

# add_executable(test_lspt_crypto
#     tests/crypto/test_lspt_crypto.cpp
# )


# target_link_libraries(test_lspt_crypto
#     PRIVATE
#     lspt_crypto
#     GTest::GTest
#     GTest::Main
# )

# add_test(NAME test_lspt_crypto COMMAND test_lspt_crypto)


# Link the test executable against GTest and lspt_core
# target_link_libraries(test_lspt_demo lspt_core ${GTEST_BOTH_LIBRARIES})



# Add the tests
# add_test(NAME LSPTDemoTest COMMAND test_lspt_demo)

# Add examples subdirectory
# add_subdirectory(examples)