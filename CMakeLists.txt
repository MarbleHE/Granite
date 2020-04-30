cmake_minimum_required (VERSION 3.8)
project(G)

# Disabled this guard until Granite & Wool are split into separate projects
#if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    enable_testing()
    include(CTest)
#endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

####################################################################################
## Build ast_lib library
####################################################################################
set(AST_OPTIMIZER_SKIP_GTEST_INSTALL true)
add_subdirectory(lib/ast)

####################################################################################
## Build granite library
####################################################################################
set(SOURCE_FILES
        src/G.cpp
        src/PadVisitor.cpp)

add_library(granite ${SOURCE_FILES})
target_include_directories(granite PUBLIC
        include
        lib/ast/include/ast
        lib/ast/include/optimizer
        lib/ast/include/utilities
        lib/ast/include/visitor
        ${Wool_LIBRARIES}
        )
target_link_libraries(granite
        ast_lib
        wool
        )

####################################################################################
## Include Tests
####################################################################################
#if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING))
    include(GoogleTest)
    add_subdirectory(tests)
#endif() #same as above - guard disabled until projects split