cmake_minimum_required(VERSION 3.8)
project(HD_sample)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(hd_sample hd.cpp)
target_include_directories(hd_sample PUBLIC
        ../../../Granite/include
        ../../../Granite/lib/SHEEP/backend/include
        ../../../Granite/lib/ast/include/ast
        ../../../Wool/include
        )
target_link_libraries(hd_sample
        granite
        ast_lib
        wool
        )
