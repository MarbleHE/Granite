cmake_minimum_required (VERSION 3.8)
project(M)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

####################################################################################
## Build ast_lib library
####################################################################################
set(AST_OPTIMIZER_SKIP_GTEST_INSTALL true)
add_subdirectory(lib/ast)

####################################################################################
## Build marble library
####################################################################################
#Linking against libraries gtest, ast, wool, sheep
set(SOURCE_FILES
        src/BitHelpers.cpp
        src/M.cpp
)

add_library(marble ${SOURCE_FILES})
target_include_directories(marble PUBLIC
        include
        lib/ast/include/ast
        lib/ast/include/optimizer
        lib/ast/include/utilities
        lib/ast/include/visitor
        ../Wool/include
        )
target_link_libraries(marble
        ast_lib
        wool
        )

####################################################################################
## Include Tests
####################################################################################
add_subdirectory(tests)