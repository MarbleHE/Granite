cmake_minimum_required(VERSION 3.8)
project(Faces_sample)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(faces_sample faces.cpp)
target_include_directories(faces_sample PUBLIC
        ../../../Granite/include
        ../../../Granite/lib/SHEEP/backend/include
        ../../../Granite/lib/ast/include/ast
        ../../../Wool/include
        )
target_link_libraries(faces_sample
        granite
        ast_lib
        wool
        )
