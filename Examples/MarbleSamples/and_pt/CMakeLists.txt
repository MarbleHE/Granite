cmake_minimum_required(VERSION 3.8)
project(And_pt_sample)

add_executable(and_pt_sample and_pt.cpp)
target_link_libraries(and_pt_sample PRIVATE Granite::granite)