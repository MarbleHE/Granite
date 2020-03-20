cmake_minimum_required(VERSION 3.8)
project(Wool)

set(CMAKE_CXX_STANDARD 17)

####################################################################################
## Build sheep library
####################################################################################
find_package(PALISADE) # Palisade includes and exposes google test to the build
if (NOT PALISADE_FOUND)
    set(APP_TESTING TRUE) # This flag decides if RPN/SHEEP's own googletest download is executed
endif()
add_subdirectory(lib/SHEEP/backend)

####################################################################################
## Add crypto libraries supported by sheep
####################################################################################
find_package(LP)
if (LP_FOUND)
    include_directories(SYSTEM ${LP_INCLUDE_DIRS})
    find_library(GMP_LIBRARIES NAMES gmp)
    add_definitions(-DHAVE_LP)
endif (LP_FOUND)

find_package(TFHE)
if (TFHE_FOUND)
    include_directories(SYSTEM ${TFHE_INCLUDE_DIRS})
    add_definitions(-DHAVE_TFHE)
endif (TFHE_FOUND)

find_package(HElib)
if (HElib_FOUND)
    include_directories(SYSTEM ${HElib_INCLUDE_DIRS})
    find_library(NTL_LIBRARIES NAMES libntl.a)
    find_library(GMP_LIBRARIES NAMES gmp)
    add_definitions(-DHAVE_HElib)
endif (HElib_FOUND)

find_package(TBB)
if (TBB_FOUND)
    link_libraries(tbb)
    include_directories(SYSTEM ${TBB_INCLUDE_DIRS})
    add_definitions(-DHAVE_TBB)
endif()

find_package(SEAL)
if (SEAL_FOUND)
    include_directories(SYSTEM ${SEAL_INCLUDE_DIRS})
    add_definitions(-DHAVE_SEAL_BFV)
    add_definitions(-DHAVE_SEAL_CKKS)
endif()

find_package(PALISADE)
if (PALISADE_FOUND)
    include_directories(SYSTEM ${PALISADE_INCLUDE_DIRS})
    link_libraries(PALISADEcore)
    add_definitions(-DHAVE_PALISADE)
endif()


####################################################################################
## Build wool library
####################################################################################
set(SOURCE_FILES
        src/Wool.cpp
        src/CircuitCompositionVisitor.cpp
        src/BatchingVisitor.cpp
        src/CircuitHelpers.cpp)

add_library(wool ${SOURCE_FILES})
target_include_directories(wool PUBLIC
        include
        lib/SHEEP/backend/include
        ../Marble/include
        ../Marble/lib/ast/include/ast
        ../Marble/lib/ast/include/visitor
        lib/SHEEP/backend/include
        #TODO: is this even necessary?
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


target_link_libraries(wool
        sheep
        ast_lib
        )


if (HElib_FOUND)
    target_link_libraries(wool ${HElib_LIBRARIES} ${NTL_LIBRARIES} ${GMP_LIBRARIES})
endif(HElib_FOUND)

if (TFHE_FOUND)
    target_link_libraries(wool ${TFHE_LIBRARIES})
endif(TFHE_FOUND)

if (SEAL_FOUND)
    target_link_libraries(wool SEAL::seal)
endif(SEAL_FOUND)

if (LP_FOUND)
    target_link_libraries(wool ${LP_LIBRARIES} ${GMP_LIBRARIES})
endif(LP_FOUND)

if (PALISADE_FOUND)
    target_link_libraries(wool ${PALISADE_LIBRARIES})
endif(PALISADE_FOUND)


####################################################################################
## Wool testing
####################################################################################
add_subdirectory(tests)
