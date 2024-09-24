set(SATELLITE_SOURCES
    srpt_satellite.cpp
)

# Create the satellite library
add_library(srpt_satellite ${SATELLITE_SOURCES})

target_include_directories(srpt_satellite PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(srpt_satellite PUBLIC srpt_core)
