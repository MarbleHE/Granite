cmake_minimum_required(VERSION 3.8)
project(Wool)

set(CMAKE_CXX_STANDARD 17)

####################################################################################
## Build sheep library
####################################################################################
add_subdirectory(lib/SHEEP/backend)
####################################################################################
## Build wool library
####################################################################################
set(SOURCE_FILES
        src/Wool.cpp
        src/CircuitCompositionVisitor.cpp
        )
add_executable(testsWool tests/testsWool.cpp
        ${SOURCE_FILES}
        )
target_include_directories(testsWool PUBLIC
        include
        lib/SHEEP/backend/include
        ../Marble/include
        ../Marble/lib/ast/include/ast
        ../Marble/lib/ast/include/visitor
        )
target_link_libraries(testsWool
        gtest_main
        sheep
        ast_lib
        )
add_test(NAME testsWool COMMAND tests)

#testing link... maybe googletest dl

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


