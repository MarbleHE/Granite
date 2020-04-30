cmake_minimum_required(VERSION 3.8)
project(HD_enc_sample)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(hd_enc_sample hd_enc.cpp)
target_include_directories(hd_enc_sample PUBLIC
        ../../../include
        ../../../Granite/lib/SHEEP/backend/include
        ../../../lib/ast/include/ast
        ../../../Wool/include
        )
target_link_libraries(hd_enc_sample
        granite
        ast_lib
        wool
        )
