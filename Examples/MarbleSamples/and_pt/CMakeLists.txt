cmake_minimum_required(VERSION 3.8)
project(And_pt_sample)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(and_pt_sample and_pt.cpp)
target_include_directories(and_pt_sample PUBLIC
        ../../../include
        ../../../Granite/lib/SHEEP/backend/include
        ../../../lib/ast/include/ast
        ../../../Wool/include
        )
target_link_libraries(and_pt_sample
        granite
        ast_lib
        wool
        )
