# src/satellite/CMakeLists.txt
set(SATELLITE_SOURCES
    lspt_satellite.cpp
)

# Create the satellite library
add_library(lspt_satellite ${SATELLITE_SOURCES})

target_include_directories(lspt_satellite PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(lspt_satellite PUBLIC lspt_core)
