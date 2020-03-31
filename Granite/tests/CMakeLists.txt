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

# this is important to have code coverage in CLion
if (CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "--coverage")
elseif ("${CMAKE_C_COMPILER_ID}" MATCHES "(Apple)?[Cc]lang" OR "${CMAKE_CXX_COMPILER_ID}" MATCHES "(Apple)?[Cc]lang")
    set(CMAKE_CXX_FLAGS "-fprofile-instr-generate -fcoverage-mapping")
endif ()


add_test(NAME testsG COMMAND tests)