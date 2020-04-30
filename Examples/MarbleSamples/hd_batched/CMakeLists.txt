cmake_minimum_required(VERSION 3.8)
project(HD_batched_sample)

add_executable(hd_batched_sample hd_batched.cpp)
target_link_libraries(hd_batched_sample PRIVATE Granite::granite)