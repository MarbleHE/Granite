cmake_minimum_required(VERSION 3.8)
project(HD_sample)

add_executable(hd_sample hd.cpp)
target_link_libraries(hd_sample PRIVATE Granite::granite)