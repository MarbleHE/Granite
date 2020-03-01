cmake_minimum_required(VERSION 3.8)
project(HD_batched)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(hd_batched hd_batched.cpp)
target_include_directories(hd_batched PUBLIC
        ../../../Marble/include
        ../../../Marble/lib/SHEEP/backend/include
        ../../../Marble/lib/ast/include/ast
        ../../../Wool/include
        )
target_link_libraries(hd_batched
        marble
        ast_lib
        wool
        )
