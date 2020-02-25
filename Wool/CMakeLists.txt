cmake_minimum_required(VERSION 3.8)
project(Wool)

set(CMAKE_CXX_STANDARD 17)

####################################################################################
## Build sheep library
####################################################################################
find_package(PALISADE) # Palisade includes and exposes google test to the build
if (NOT PALISADE_FOUND)
    set(APP_TESTING TRUE) # This flag decides if RPN/SHEEP's own googletest download is executed
endif()
add_subdirectory(lib/SHEEP/backend)

####################################################################################
## Build wool library
####################################################################################
set(SOURCE_FILES
        src/Wool.cpp
        src/CircuitCompositionVisitor.cpp
        )

add_library(wool ${SOURCE_FILES})
target_include_directories(wool PUBLIC
        include
        lib/SHEEP/backend/include
        ../Marble/include
        ../Marble/lib/ast/include/ast
        ../Marble/lib/ast/include/visitor
        )
target_link_libraries(wool
        sheep
        ast_lib
        )

####################################################################################
## Wool testing
####################################################################################
add_subdirectory(tests)
