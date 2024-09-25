#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "SRPT::srpt-protocol" for configuration ""
set_property(TARGET SRPT::srpt-protocol APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(SRPT::srpt-protocol PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "CXX"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libsrpt-protocol.a"
  )

list(APPEND _cmake_import_check_targets SRPT::srpt-protocol )
list(APPEND _cmake_import_check_files_for_SRPT::srpt-protocol "${_IMPORT_PREFIX}/lib/libsrpt-protocol.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
