find_path(SEAL_INCLUDE_DIR NAMES seal
        PATHS /SEAL/native/src/
        )
find_library(SEAL_LIBRARY NAMES libseal-3.4.a
        PATHS /SEAL/native/lib/
        )

#set(SEAL_FOUND TRUE)
set(SEAL_INCLUDE_DIRS ${SEAL_INCLUDE_DIR})
set(SEAL_LIBRARIES ${SEAL_LIBRARY})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(SEAL DEFAULT_MSG
                                  SEAL_LIBRARY SEAL_INCLUDE_DIR)

mark_as_advanced(SEAL_LIBRARY SEAL_INCLUDE_DIR)
