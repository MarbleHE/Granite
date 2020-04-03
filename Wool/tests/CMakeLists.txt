cmake_minimum_required(VERSION 3.14)
####################################################################################
## Wool testing
####################################################################################
include(GoogleTest)
add_executable(testsWool testsWool.cpp)
target_include_directories(testsWool PUBLIC
        ../include
        ../lib/SHEEP/backend/include
        ../../Granite/include
        ../../Granite/lib/ast/include/ast
        ../../Granite/lib/ast/include/visitor
        )
target_link_libraries(testsWool
        gtest_main
        sheep
        ast_lib
        wool
        granite
        )


add_test(NAME testsWool COMMAND tests)