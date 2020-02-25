####################################################################################
## Wool testing
####################################################################################
add_executable(testsWool testsWool.cpp)
target_include_directories(testsWool PUBLIC
        ../include
        ../lib/SHEEP/backend/include
        ../../Marble/include
        ../../Marble/lib/ast/include/ast
        ../../Marble/lib/ast/include/visitor
        )
target_link_libraries(testsWool
        gtest_main
        sheep
        ast_lib
        )
add_test(NAME testsWool COMMAND tests)