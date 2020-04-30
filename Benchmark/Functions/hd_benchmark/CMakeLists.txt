cmake_minimum_required(VERSION 3.8)
project(HD_benchmark)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(hd_benchmark hd_benchmark.cpp)
target_include_directories(hd_benchmark PUBLIC
        ../../../include
        ../../../Granite/lib/SHEEP/backend/include
        ../../../lib/ast/include/ast
        ../../../Wool/include
        )
target_link_libraries(hd_benchmark
        granite
        ast_lib
        wool
        )

add_executable(hd_plain_benchmark hd.cpp)
set_target_properties(hd_plain_benchmark PROPERTIES COMPILE_FLAGS "-O3 -march=native -fopenmp-simd")
target_include_directories(hd_plain_benchmark PUBLIC
        ../../../include
        ../../../Granite/lib/SHEEP/backend/include
        ../../../lib/ast/include/ast
        ../../../Wool/include
        )
target_link_libraries(hd_plain_benchmark
        granite
        ast_lib
        wool
        )