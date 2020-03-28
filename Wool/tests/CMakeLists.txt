####################################################################################
## Wool testing
####################################################################################
add_executable(testsWool testsWool.cpp)
target_include_directories(testsWool PUBLIC
        ../include
        ../lib/SHEEP/backend/include
        ../../Granite/include
        ../../Granite/lib/ast/include/ast
        ../../Granite/lib/ast/include/visitor

        lib/SHEEP/backend/lib/HElib/src
        lib/SHEEP/backend/lib/HElib/src/misc
        lib/SHEEP/backend/lib/HElib/src/aes
        lib/SHEEP/backend/lib/palisade-release/src/abe/include
        lib/SHEEP/backend/lib/palisade-release/src/binfhe/include
        lib/SHEEP/backend/lib/palisade-release/src/core/include
        lib/SHEEP/backend/lib/palisade-release/src/pke/include
        lib/SHEEP/backend/lib/palisade-release/src/signature/include
        lib/SHEEP/backend/lib/SEAL/native/src
        lib/SHEEP/backend/lib/SEAL/native/src/seal/util
        lib/SHEEP/backend/lib/tfhe/src/include
        )
target_link_libraries(testsWool
        gtest_main
        sheep
        ast_lib
        wool
        granite
        )
add_test(NAME testsWool COMMAND tests)