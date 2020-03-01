cmake_minimum_required(VERSION 3.8)
project(HD_enc)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(hd_enc hd_enc.cpp)
target_include_directories(hd_enc PUBLIC
        ../../../Marble/include
        ../../../Marble/lib/SHEEP/backend/include
        ../../../Marble/lib/ast/include/ast
        ../../../Wool/include
        )
target_link_libraries(hd_enc
        marble
        ast_lib
        wool
        )
