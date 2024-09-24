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

# Create the lspt-protocol library target
add_library(lspt-protocol STATIC)

# Add subdirectories and collect their sources
add_subdirectory(src/core)
add_subdirectory(src/crypto)
add_subdirectory(src/congestion_control)
add_subdirectory(src/common)
add_subdirectory(src/space)
add_subdirectory(src/ground)
add_subdirectory(src/network)
add_subdirectory(src/ai)
add_subdirectory(src/satellite)

# Collect all sources from subdirectories
file(GLOB_RECURSE LSPT_SOURCES 
    "src/*.cpp"
)

# Add sources to the lspt-protocol target
target_sources(lspt-protocol PRIVATE ${LSPT_SOURCES})

# Link libraries to lspt-protocol
target_link_libraries(lspt-protocol
    PUBLIC
    OpenSSL::SSL
    OpenSSL::Crypto
    ${LIBSODIUM_LIBRARY}
    nlohmann_json::nlohmann_json
)

# Set include directories for lspt-protocol
target_include_directories(lspt-protocol
    PUBLIC
    ${PROJECT_SOURCE_DIR}/src
    ${LIBSODIUM_INCLUDE_DIRS}
)

# Propagate compile options and link directories
target_compile_options(lspt-protocol PUBLIC ${LIBSODIUM_CFLAGS_OTHER})
target_link_directories(lspt-protocol PUBLIC ${LIBSODIUM_LIBRARY_DIRS})

# Include the tests directory
add_subdirectory(tests)
add_subdirectory(tests/core)
add_subdirectory(tests/crypto)
add_subdirectory(tests/congestion_control)
add_subdirectory(tests/integration)
add_subdirectory(tests/network)
add_subdirectory(tests/satelite)
# Enable testing
enable_testing()