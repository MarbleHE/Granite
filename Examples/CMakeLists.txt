cmake_minimum_required (VERSION 3.8)
project(M)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(example example.cpp)
target_include_directories(example PUBLIC
        ../Marble/include
        ../Marble/lib/SHEEP/backend/include
        ../Marble/lib/ast/include/ast
        ../Wool/include
        )
target_link_libraries(example
        marble
        ast_lib
        wool
        )
