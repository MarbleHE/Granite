cmake_minimum_required(VERSION 3.8)
project(HD_enc_sample)

add_executable(hd_enc_sample hd_enc.cpp)
target_link_libraries(hd_enc_sample PRIVATE Granite::granite)