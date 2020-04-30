cmake_minimum_required (VERSION 3.8)
project(Granite
        VERSION 0.1
        DESCRIPTION "Granite is a compiler for FHE"
        LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/Modules")

####################################################################################
## Dependencies
####################################################################################
find_package(Wool REQUIRED)

####################################################################################
## Build granite library
####################################################################################
set(SOURCE_FILES
        src/G.cpp
        src/PadVisitor.cpp)

SET(PUBLIC_HEADERS
        include/G.hpp)

add_library(granite ${SOURCE_FILES})
add_library(Granite::granite ALIAS granite)
target_include_directories(granite PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)
set_target_properties(granite PROPERTIES PUBLIC_HEADER ${PUBLIC_HEADERS})
target_compile_features(granite PUBLIC cxx_std_17)
target_link_libraries(granite PUBLIC Wool::wool)

####################################################################################
## Granite installation
##
## granite installed globally as a library
####################################################################################
# Create a Version File
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
        GraniteConfigVersion.cmake
        VERSION ${PACKAGE_VERSION}
        COMPATIBILITY AnyNewerVersion
)

# Configure (i.e. generate) the GraniteConfig.cmake file and copy both it and the version to install
configure_file(GraniteConfig.cmake.in GraniteConfig.cmake @ONLY)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/GraniteConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/GraniteConfigVersion.cmake"
        DESTINATION lib/cmake/Granite)

install(TARGETS granite
        EXPORT  GraniteTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        PUBLIC_HEADER DESTINATION include
        INCLUDES DESTINATION include)

install(EXPORT GraniteTargets
        FILE GraniteTargets.cmake
        NAMESPACE Wool::
        DESTINATION lib/cmake/Granite)

####################################################################################
## Granite Examples
####################################################################################
add_subdirectory(examples)

####################################################################################
## Granite Benchmarking
####################################################################################
add_subdirectory(benchmark)

####################################################################################
## Granite Testing
####################################################################################
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    enable_testing()
    add_subdirectory(tests EXCLUDE_FROM_ALL)
endif()