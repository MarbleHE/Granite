cmake_minimum_required(VERSION 3.8)
project(Faces_sample)

add_executable(faces_sample faces.cpp)
target_link_libraries(faces_sample PRIVATE Granite::granite)