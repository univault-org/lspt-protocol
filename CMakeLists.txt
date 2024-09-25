cmake_minimum_required(VERSION 3.10)
project(SRPTProtocol)
project(SRPTProtocol VERSION 1.0.0)

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

# Create the srpt-protocol library target
add_library(srpt-protocol STATIC)

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
file(GLOB_RECURSE SRPT_SOURCES 
    "src/*.cpp"
)

# Add sources to the srpt-protocol target
target_sources(srpt-protocol PRIVATE ${SRPT_SOURCES})

# Link libraries to srpt-protocol
target_link_libraries(srpt-protocol
    PUBLIC
    OpenSSL::SSL
    OpenSSL::Crypto
    ${LIBSODIUM_LIBRARY}
    nlohmann_json::nlohmann_json
)

# Set include directories for srpt-protocol
target_include_directories(srpt-protocol
    PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:include>
    ${LIBSODIUM_INCLUDE_DIRS}
)

# Propagate compile options and link directories
target_compile_options(srpt-protocol PUBLIC ${LIBSODIUM_CFLAGS_OTHER})
target_link_directories(srpt-protocol PUBLIC ${LIBSODIUM_LIBRARY_DIRS})

# Installation rules
include(GNUInstallDirs)
install(TARGETS srpt-protocol
    EXPORT srpt-protocol-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Install nlohmann_json
install(TARGETS nlohmann_json
    EXPORT srpt-protocol-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Install header files
install(DIRECTORY 
    ${PROJECT_SOURCE_DIR}/src/
    ${PROJECT_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/srpt-protocol
    FILES_MATCHING PATTERN "*.h"
)

# Export targets
install(EXPORT srpt-protocol-targets
    FILE srpt-protocol-targets.cmake
    NAMESPACE SRPT::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/srpt-protocol
)

# Create a ConfigVersion.cmake file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/srpt-protocol-config-version.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

# Create a Config.cmake file
configure_file(cmake/srpt-protocol-config.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/srpt-protocol-config.cmake"
    @ONLY
)

# Install the config files
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/srpt-protocol-config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/srpt-protocol-config-version.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/srpt-protocol
)
# Include the tests directory
add_subdirectory(tests)
add_subdirectory(tests/core)
add_subdirectory(tests/crypto)
add_subdirectory(tests/congestion_control)
add_subdirectory(tests/integration)
add_subdirectory(tests/network)
add_subdirectory(tests/satellite)
# Enable testing
enable_testing()