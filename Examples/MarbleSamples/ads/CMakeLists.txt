cmake_minimum_required(VERSION 3.5)
project(Ads_sample)

add_executable(ads_sample ads.cpp)
target_link_libraries(ads_sample PRIVATE Granite::granite)