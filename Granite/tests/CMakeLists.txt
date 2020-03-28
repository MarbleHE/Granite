cmake_minimum_required(VERSION 3.14)
##############################
# TARGET: testing
##############################
add_executable(testsG testsG.cpp)
target_include_directories(testsG PUBLIC
        ../include
        ../lib/ast/include/ast
        ../lib/ast/include/optimizer
        ../lib/ast/include/utilities
        ../lib/ast/include/visitor
        ../../Wool/include
        )
target_link_libraries(testsG
        granite
        gtest_main
        ast_lib
        wool
        )
add_test(NAME testsG COMMAND tests)