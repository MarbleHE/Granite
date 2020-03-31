find_path(PALISADE_INCLUDE_DIR NAMES palisade.h
        PATHS /palisade-release/src/pke/include
        )
find_library(PALISADE_CORE_LIBRARY NAMES libPALISADEcore
        PATHS /palisade-release/build/lib
        )
find_library(PALISADE_PKE_LIBRARY NAMES libPALISADEpke
        PATHS /palisade-release/build/lib
        )
find_library(PALISADE_NTL_LIBRARY NAMES ntl.a
        PATHS /ntl-11.1.0/src/
        )

#set(PALISADE_FOUND TRUE)
set(PALISADE_INCLUDE_DIRS ${PALISADE_INCLUDE_DIR})
set(PALISADE_LIBRARIES ${PALISADE_CORE_LIBRARY} ${PALISADE_PKE_LIBRARY} ${PALISADE_NTL_LIBRARY})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(PALISADE DEFAULT_MSG
                                  PALISADE_CORE_LIBRARY PALISADE_INCLUDE_DIR)

mark_as_advanced(PALISADE_CORE_LIBRARY PALISADE_PKE_LIBRARY PALISADE_NTL_LIBRARY PALISADE_INCLUDE_DIR)
