cmake_minimum_required(VERSION 3.5)
set(TEST_PROJECT_NAME ${PROJECT_NAME}_tests)

set(CPPUTEST_HOME /home/ubuntu/cpputest)

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

set(TEST_SOURCES
        src/test_basicTX.cpp
        src/AllTests.cpp
        fakes/fake_vertices_errors.c
        vertices_ports/http_curl.c
        )

set(INC_FOLDERS
        config
        )

# check that BLOCKCHAIN_PROVIDER variable is set, default to `algorand`
if(DEFINED ENV{BLOCKCHAIN_PROVIDER})
    message(STATUS "Using blockchain: $ENV{BLOCKCHAIN_PROVIDER}")
else()
    set(ENV{BLOCKCHAIN_PROVIDER} algorand)
    message(STATUS "Default blockchain: $ENV{BLOCKCHAIN_PROVIDER}")
endif()

import_libs()
message(${CPPUTEST_INCLUDE_DIRS})
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

# Find cURL
find_package(CURL)
include_directories(${CURL_INCLUDE_DIRS})

# Take care of include directories
include_directories(${CPPUTEST_INCLUDE_DIRS})
message("I am here")
message(${CPPUTEST_LIBRARIES})
link_directories(${CPPUTEST_LIBRARIES})

# Import Vertices
add_library(Vertices::vertices STATIC IMPORTED)

include_directories(${INC_FOLDERS})
add_executable(${TEST_PROJECT_NAME} ${TEST_SOURCES})
target_link_libraries(${TEST_PROJECT_NAME} sodium curl vertices ${CPPUTEST_LDFLAGS})
