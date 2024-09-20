cmake_minimum_required(VERSION 3.10)
project(LSPTProtocol)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find OpenSSL
find_package(OpenSSL REQUIRED)

# Find GTest
find_package(GTest REQUIRED)

# Find PkgConfig and libsodium
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBSODIUM REQUIRED libsodium)

# Use find_library to locate the libsodium library
find_library(LIBSODIUM_LIBRARY
    NAMES sodium libsodium
    PATHS ${LIBSODIUM_LIBRARY_DIRS} /usr/local/lib /usr/lib /usr/local/Cellar/libsodium/1.0.20/lib
    NO_DEFAULT_PATH
)

if(NOT LIBSODIUM_LIBRARY)
    message(FATAL_ERROR "libsodium library not found. Please check your installation.")
endif()

message(STATUS "Found libsodium: ${LIBSODIUM_LIBRARY}")

# Include directories
include_directories(${GTEST_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${LIBSODIUM_INCLUDE_DIRS})

# Add subdirectories
add_subdirectory(src/core)
add_subdirectory(src/crypto)
add_subdirectory(src/congestion_control)
add_subdirectory(tests/core)
add_subdirectory(tests/crypto)
add_subdirectory(tests/congestion_control)
# Enable testing
enable_testing()

# Link directories
link_directories(${LIBSODIUM_LIBRARY_DIRS})

# Add these lines to propagate the include and library directories
add_compile_options(${LIBSODIUM_CFLAGS_OTHER})
include_directories(${LIBSODIUM_INCLUDE_DIRS})
link_directories(${LIBSODIUM_LIBRARY_DIRS})

set(LIBSODIUM_ROOT_DIR "/usr/local/Cellar/libsodium/1.0.20")
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${LIBSODIUM_ROOT_DIR})

find_path(LIBSODIUM_INCLUDE_DIR sodium.h
          PATHS ${LIBSODIUM_ROOT_DIR}/include
          NO_DEFAULT_PATH)

set(LIBSODIUM_LIBRARY "${LIBSODIUM_ROOT_DIR}/lib/libsodium.dylib")

if(NOT LIBSODIUM_INCLUDE_DIR OR NOT EXISTS ${LIBSODIUM_LIBRARY})
  message(FATAL_ERROR "libsodium not found. Please check the installation path.")
endif()

include_directories(${LIBSODIUM_INCLUDE_DIR})
link_directories(${LIBSODIUM_ROOT_DIR}/lib)