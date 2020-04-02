cmake_minimum_required(VERSION 3.8)
project(And_batched)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(and_batched and_batched.cpp)
target_include_directories(and_batched PUBLIC
        ../../../Granite/include
        ../../../Granite/lib/SHEEP/backend/include
        ../../../Granite/lib/ast/include/ast
        ../../../Wool/include
        )
target_link_libraries(and_batched
        granite
        ast_lib
        wool
        )
