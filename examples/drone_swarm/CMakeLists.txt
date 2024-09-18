cat << EOF > examples/drone_swarm/CMakeLists.txt
add_executable(drone_swarm_demo
    src/drone.cpp
    src/local_control_station.cpp
    src/satellite.cpp
)

target_link_libraries(drone_swarm_demo lspt_core)

# Tests for the drone swarm example
add_subdirectory(tests)
EOF