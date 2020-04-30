cmake_minimum_required(VERSION 3.8)
project(HD_benchmark)

add_executable(hd_benchmark hd_benchmark.cpp)
target_link_libraries(hd_benchmark PIVATE Granite::granite)

add_executable(hd_plain_benchmark hd.cpp)
set_target_properties(hd_plain_benchmark PROPERTIES COMPILE_FLAGS "-O3 -march=native -fopenmp-simd")
target_link_libraries(hd_plain_benchmark Granite::granite)