cmake_minimum_required(VERSION 3.8)
project(And_enc_sample)

add_executable(and_enc_sample and_enc.cpp)
target_link_libraries(and_enc_sample PRIVATE Granite::granite)