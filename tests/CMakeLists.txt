cmake_minimum_required(VERSION 3.5)

project(VerticesTop C CXX)
set(CPPUTEST_HOME /home/ubuntu/cpputest)
get_filename_component(VERTICES_TEST_ROOT . ABSOLUTE)
set(VERTICES_ROOT ${VERTICES_TEST_ROOT}/..)
add_definitions(-DVERTICES_ROOT="${VERTICES_ROOT}")

# Module path to find libs
set(CMAKE_MODULE_PATH ${VERTICES_ROOT}/utils/)

set(INC_FOLDERS
        config
        )

## CPPUTEST Install ENV

if(DEFINED CPPUTEST_HOME)
    message(STATUS "Using CppUTest home: ${CPPUTEST_HOME}")
    set(CPPUTEST_INCLUDE_DIRS ${CPPUTEST_HOME}/include)
    set(CPPUTEST_LIBRARIES ${CPPUTEST_HOME}/cpputest_build/lib)
    set(CPPUTEST_LDFLAGS CppUTest CppUTestExt)
else()
    find_package(PkgConfig REQUIRED)
    pkg_search_module(CPPUTEST REQUIRED cpputest>=3.8)
    message(STATUS "Found CppUTest version ${CPPUTEST_VERSION}")
endif()
## CPPUTEST END

# check that BLOCKCHAIN_PROVIDER variable is set, default to `algorand`
if(DEFINED ENV{BLOCKCHAIN_PROVIDER})
    message(STATUS "Using blockchain: $ENV{BLOCKCHAIN_PROVIDER}")
else()
    set(ENV{BLOCKCHAIN_PROVIDER} algorand)
    message(STATUS "Default blockchain: $ENV{BLOCKCHAIN_PROVIDER}")
endif()

add_definitions(-DCONFIG_PATH="${CMAKE_CURRENT_SOURCE_DIR}/config/")
string(LENGTH "${CMAKE_SOURCE_DIR}/" SOURCE_PATH_SIZE)
add_definitions("-DSOURCE_PATH_SIZE=${SOURCE_PATH_SIZE}")

if ($ENV{DOCKER_RUN})
    # From Docker (CI jobs) we want to send only 100 micro algos to Bob
    # so that we don't have to put money into Alice's account too often
    add_definitions("-DAMOUNT_SENT=100")
else()
    # Generic example sends 1 algo
    add_definitions("-DAMOUNT_SENT=1000000")
endif()

include(../utils/utils.cmake)
message(STATUS "💎 Loading targets")

# default behaviour is to compile Unix example if current CMake file is used as
# project root CMake
if (CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set(COMPILE_UNIX ON)
endif ()

if (COMPILE_UNIX)
    message(STATUS "Vertices SDK examples: unix")

    set(CMAKE_MACOSX_RPATH 1)

    include(../utils/libs.cmake)

    add_subdirectory(../examples/unix ./bin/unix EXCLUDE_FROM_ALL)
    add_subdirectory(../external/mbedtls ./bin/mbedtls EXCLUDE_FROM_ALL)
    add_subdirectory(../external/cJSON ./bin/cJSON EXCLUDE_FROM_ALL)
else ()
    message(STATUS "Vertices SDK examples: generic")

    # build CJSON static lib insteado of default shared lib
    set(CJSON_OVERRIDE_BUILD_SHARED_LIBS ON)
    set(CJSON_BUILD_SHARED_LIBS OFF)

    # Let's use the mbedtls and cJSON library provided with the SDK
    add_subdirectory(../external/mbedtls)
    add_subdirectory(../external/cJSON)
endif ()

import_libs()

# Import Vertices
add_subdirectory(../src ./bin/src EXCLUDE_FROM_ALL)
#add_library(Vertices::vertices STATIC IMPORTED)

# Take care of include directories
include_directories(${CPPUTEST_INCLUDE_DIRS})
message(${CPPUTEST_LIBRARIES})
link_directories(${CPPUTEST_LIBRARIES})

# Find cURL
find_package(CURL)
include_directories(${CURL_INCLUDE_DIRS})

include_directories(${INC_FOLDERS})

add_subdirectory(src/BasicTX)
add_subdirectory(src/Vertices)

