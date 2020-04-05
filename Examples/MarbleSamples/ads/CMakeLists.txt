cmake_minimum_required(VERSION 3.5)
project(Ads_sample)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(ads_sample ads.cpp)
target_include_directories(ads_sample PUBLIC
        ../../../Granite/include
        ../../../Granite/lib/SHEEP/backend/include
        ../../../Granite/lib/ast/include/ast
        ../../../Wool/include
        )
target_link_libraries(ads_sample
        granite
        ast_lib
        wool
        )
