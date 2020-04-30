cmake_minimum_required(VERSION 3.8)
project(HD_batched_sample)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(hd_batched_sample hd_batched.cpp)
target_include_directories(hd_batched_sample PUBLIC
        ../../../include
        ../../../Granite/lib/SHEEP/backend/include
        ../../../lib/ast/include/ast
        ../../../Wool/include
        )
target_link_libraries(hd_batched_sample
        granite
        ast_lib
        wool
        )
