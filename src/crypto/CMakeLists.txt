# src/crypto/CMakeLists.txt
add_library(lspt_crypto
    lspt_crypto.cpp
)

target_include_directories(lspt_crypto PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(lspt_crypto PUBLIC ${LIBSODIUM_LIBRARY})