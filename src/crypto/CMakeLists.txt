# src/crypto/CMakeLists.txt
add_library(srpt_crypto
    srpt_crypto.cpp
)

target_include_directories(srpt_crypto PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(srpt_crypto PUBLIC ${LIBSODIUM_LIBRARY})