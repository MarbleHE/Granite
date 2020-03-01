cmake_minimum_required(VERSION 3.8)
project(Ads)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(ads ads.cpp)
target_include_directories(ads PUBLIC
        ../../../Marble/include
        ../../../Marble/lib/SHEEP/backend/include
        ../../../Marble/lib/ast/include/ast
        ../../../Wool/include
        )
target_link_libraries(ads
        marble
        ast_lib
        wool
        )
