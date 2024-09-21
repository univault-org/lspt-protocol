cmake_minimum_required(VERSION 3.10)
project(LSPTProtocol)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(OpenSSL REQUIRED)
find_package(GTest REQUIRED)
find_package(PkgConfig REQUIRED)

# Configure libsodium
pkg_check_modules(LIBSODIUM REQUIRED libsodium)
find_library(LIBSODIUM_LIBRARY
    NAMES sodium libsodium
    PATHS ${LIBSODIUM_LIBRARY_DIRS} /usr/local/lib /usr/lib /usr/local/Cellar/libsodium/1.0.20/lib
    NO_DEFAULT_PATH
)

if(NOT LIBSODIUM_LIBRARY)
    message(FATAL_ERROR "libsodium library not found. Please check your installation.")
endif()

message(STATUS "Found libsodium: ${LIBSODIUM_LIBRARY}")

# Include FetchContent module for nlohmann_json
include(FetchContent)
FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz
    DOWNLOAD_EXTRACT_TIMESTAMP true
)
FetchContent_MakeAvailable(json)

# Include directories
include_directories(
    ${GTEST_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/src
    ${LIBSODIUM_INCLUDE_DIRS}
)

# Add subdirectories
add_subdirectory(src/core)
add_subdirectory(src/crypto)
add_subdirectory(src/congestion_control)
add_subdirectory(src/common)
add_subdirectory(src/space)
add_subdirectory(src/ground)
add_subdirectory(src/network)
add_subdirectory(src/ai)

# Include the tests directory
add_subdirectory(tests)
add_subdirectory(tests/core)
add_subdirectory(tests/crypto)
add_subdirectory(tests/congestion_control)
add_subdirectory(tests/integration)
add_subdirectory(tests/network)


# Enable testing
enable_testing()

# Propagate compile options and link directories
add_compile_options(${LIBSODIUM_CFLAGS_OTHER})
link_directories(${LIBSODIUM_LIBRARY_DIRS})