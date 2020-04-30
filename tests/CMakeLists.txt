cmake_minimum_required(VERSION 3.14)
include(FetchContent) # Introduced in CMake 3.11
include(GoogleTest) # Introduced in CMake 3.10

##############################
# Download GoogleTest framework
##############################
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.10.0
)
FetchContent_MakeAvailable(googletest)

##############################
# TARGET: testing
##############################
add_executable(testsGranite testsG.cpp)

# this is important to have code coverage in CLion
if (CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "--coverage")
elseif ("${CMAKE_C_COMPILER_ID}" MATCHES "(Apple)?[Cc]lang" OR "${CMAKE_CXX_COMPILER_ID}" MATCHES "(Apple)?[Cc]lang")
    set(CMAKE_CXX_FLAGS "-fprofile-instr-generate -fcoverage-mapping")
endif ()

target_link_libraries(testsGranite PRIVATE granite gtest gtest_main)
target_include_directories(testsGranite PRIVATE ../include/)

# create ctest targets
gtest_discover_tests(testsGranite TEST_PREFIX gtest:)

# make sure that wool is built before the testing-all target
add_dependencies(testsGranite granite)