   

   include(CMakeFindDependencyMacro)

   find_dependency(OpenSSL REQUIRED)
   find_dependency(PkgConfig REQUIRED)

   pkg_check_modules(LIBSODIUM REQUIRED libsodium)
   find_library(LIBSODIUM_LIBRARY
       NAMES sodium libsodium
       PATHS ${LIBSODIUM_LIBRARY_DIRS} /usr/local/lib /usr/lib /usr/local/Cellar/libsodium/1.0.20/lib
       NO_DEFAULT_PATH
   )

   if(NOT LIBSODIUM_LIBRARY)
       message(FATAL_ERROR "libsodium library not found. Please check your installation.")
   endif()

   include("${CMAKE_CURRENT_LIST_DIR}/srpt-protocol-targets.cmake")
