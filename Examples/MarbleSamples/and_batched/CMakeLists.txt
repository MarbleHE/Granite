cmake_minimum_required(VERSION 3.8)
project(And_batched_sample)

add_executable(and_batched_sample and_batched.cpp)
target_link_libraries(and_batched_sample PRIVATE Granite::granite)