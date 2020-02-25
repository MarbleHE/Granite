cmake_minimum_required(VERSION 3.14)
##############################
# TARGET: testing
##############################
add_executable(testsM testsM.cpp)
target_include_directories(testsM PUBLIC
        ../include
        ../lib/ast/include/ast
        ../lib/ast/include/optimizer
        ../lib/ast/include/utilities
        ../lib/ast/include/visitor
        ../../Wool/include
        )
target_link_libraries(testsM
        marble
        gtest_main
        ast_lib
        wool
        )
add_test(NAME testsM COMMAND tests)