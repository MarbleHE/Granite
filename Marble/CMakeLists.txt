cmake_minimum_required (VERSION 3.8)
project(M)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

####################################################################################
## Build master_thesis_code_lib library
####################################################################################
add_subdirectory(lib/ast)

####################################################################################
## Tests for M
####################################################################################
#[[
# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
        ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
        EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include")
endif()
]]

#Linking against libraries gtest, ast, wool, sheep
set(SOURCE_FILES
        src/BitHelpers.cpp
        src/M.cpp
)

add_executable(testsM tests/testsM.cpp
        ${SOURCE_FILES}
        )
target_include_directories(testsM PUBLIC
        include
        lib/ast/include/ast
        lib/ast/innclude/optimizer
        lib/ast/include/utilities
        lib/ast/include/visitor)
target_link_libraries(testsM
        gtest_main
        ast_lib
        )

#target_link_libraries(testsM sheep)


add_test(NAME testsM COMMAND tests)

##############################
# TARGET: testing
##############################
#[[
set(TEST_FILES
        tests.cpp
        )

add_executable(testing-all
        main.cpp
        ${TEST_FILES})

# this is important to have code coverage in CLion
set(CMAKE_CXX_FLAGS "-fprofile-instr-generate -fcoverage-mapping")

target_link_libraries(testing-all gtest)
]]