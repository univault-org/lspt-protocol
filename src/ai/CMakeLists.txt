# Collect all the source files in this directory
set(AI_DATA_SOURCES
    data/vectorized_data_handler.cpp
)

# Create a library target for the AI data handling
add_library(ai_data STATIC ${AI_DATA_SOURCES})

# Specify include directories for this library
target_include_directories(ai_data PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Set C++ standard for this target
target_compile_features(ai_data PUBLIC cxx_std_17)

# Check if nlohmann_json target exists
if(TARGET nlohmann_json::nlohmann_json)
    target_link_libraries(ai_data PUBLIC nlohmann_json::nlohmann_json)
else()
    message(WARNING "nlohmann_json target not found. Make sure it's properly included in your root CMakeLists.txt")
    # Fallback: try to find the library manually
    find_package(nlohmann_json QUIET)
    if(nlohmann_json_FOUND)
        target_link_libraries(ai_data PUBLIC nlohmann_json::nlohmann_json)
    else()
        message(WARNING "nlohmann_json not found. AI data functionality might be limited.")
    endif()
endif()