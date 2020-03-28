cmake_minimum_required(VERSION 3.8)
project(Examples)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(example example.cpp)
target_include_directories(example PUBLIC
        ../Granite/include
        ../Granite/lib/SHEEP/backend/include
        ../Granite/lib/ast/include/ast
        ../Wool/include
        )
target_link_libraries(example
        marble
        ast_lib
        wool
        )

add_subdirectory(MarbleSamples)