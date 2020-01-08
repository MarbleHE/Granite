cmake_minimum_required (VERSION 3.8)
project(M)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


####################################################################################
## Tests
####################################################################################

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

# Now simply link against gtest or gtest_main as needed. Eg
add_executable(testsM tests/testsM.cpp
        src/M.cpp)
target_include_directories(testsM PUBLIC
        include)
target_link_libraries(testsM gtest_main)

#target_link_libraries(testsM sheep)

if (HElib_FOUND)
    target_link_libraries(rpn ${HElib_LIBRARIES} ${NTL_LIBRARIES} ${GMP_LIBRARIES})
endif(HElib_FOUND)

if (TFHE_FOUND)
    target_link_libraries(rpn ${TFHE_LIBRARIES})
endif(TFHE_FOUND)

if (SEAL_FOUND)
    target_link_libraries(rpn ${SEAL_LIBRARIES})
endif(SEAL_FOUND)

if (LP_FOUND)
    target_link_libraries(rpn ${LP_LIBRARIES} ${GMP_LIBRARIES})
endif(LP_FOUND)

if (PALISADE_FOUND)
    target_link_libraries(rpn ${PALISADE_LIBRARIES})
endif(PALISADE_FOUND)

add_test(NAME rpn_tests COMMAND tests)
