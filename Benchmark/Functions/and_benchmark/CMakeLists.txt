cmake_minimum_required(VERSION 3.8)
project(And_benchmark)

add_executable(and_benchmark and_benchmark.cpp)
target_link_libraries(and_benchmark PRIVATE Granite::granite)