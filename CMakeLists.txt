cmake_minimum_required(VERSION 3.5)
set(PROJECT_NAME Vertices-SDK)
project(${PROJECT_NAME})

# Set variable VERTICES_ROOT to absolute path of that file location
get_filename_component(VERTICES_ROOT . ABSOLUTE)
add_definitions(-DVERTICES_ROOT="${VERTICES_ROOT}")

include(utils/utils.cmake)

message(STATUS "💎 Loading targets")

# Module path to find libs
set(CMAKE_MODULE_PATH ${VERTICES_ROOT}/utils/)

# Set blockchain
set(ENV{BLOCKCHAIN_PROVIDER} algorand)

# default behaviour is to compile Unix example if current CMake file is used as
# project root CMake
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set(COMPILE_UNIX ON)
endif()

if (COMPILE_UNIX)
    message(STATUS "Vertices SDK examples: unix")

    set(CMAKE_MACOSX_RPATH 1)

    include(utils/libs.cmake)

    add_subdirectory(examples/unix)
    add_subdirectory(external/mbedtls)
    add_subdirectory(external/cJSON)
else()
    message(STATUS "Vertices SDK examples: generic")

    # build CJSON static lib insteado of default shared lib
    set(CJSON_OVERRIDE_BUILD_SHARED_LIBS ON)
    set(CJSON_BUILD_SHARED_LIBS OFF)

    # Let's use the mbedtls and cJSON library provided with the SDK
    add_subdirectory(external/mbedtls)
    add_subdirectory(external/cJSON)
endif ()

# Add vertices library
add_subdirectory(src)