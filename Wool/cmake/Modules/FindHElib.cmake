find_path(HElib_INCLUDE_DIRS NAMES FHE.h PATH_SUFFIXES helib)
find_library(HElib_LIBRARIES NAMES libhelib.a)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(HElib DEFAULT_MSG
        HElib_LIBRARIES HElib_INCLUDE_DIRS)

mark_as_advanced(HElib_LIBRARIES HElib_INCLUDE_DIRS)

