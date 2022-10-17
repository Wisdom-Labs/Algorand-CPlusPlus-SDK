set(PROJECT_NAME unix_example)
project(${PROJECT_NAME} C CXX)
if (NOT MSVC)
    set (CMAKE_CXX_FLAGS "-std=c++11")
endif()
set(SRC_FILES
        main.cpp
        vertices_ports/http_curl.cpp
        )

set(INC_FOLDERS
        config
        )

import_libs()

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

# lots of warnings and all warnings as errors
if(NOT MSVC)
    add_compile_options(
            -pedantic
            -Wall
            -Wstrict-prototypes
            -Wwrite-strings
            -Wshadow
            -Winit-self
            -Wcast-align
            -Wformat=2
            -Wmissing-prototypes
            -Wstrict-overflow=2
            -Wc++-compat
            -Wundef
            -Wswitch-default
            -Wconversion
            -Wno-vla-extension
            -Wno-missing-braces)
endif ()

if(MSVC OR MSYS OR MINGW)
    # DLL ,  Link CURL & SODIUM on Windows
    # please download & install sodium and curl and set environment variables
    set(libcurl_INCLUDE_DIRS "C:/libcurl-7.85/include")
    include_directories(${libcurl_INCLUDE_DIRS})

    set(libsodium_INCLUDE_DIRS "C:/libsodium-1.0.17/include")
    include_directories(${libsodium_INCLUDE_DIRS})

    include_directories(${INC_FOLDERS})
    add_executable(${PROJECT_NAME} ${SRC_FILES})
    target_link_libraries(${PROJECT_NAME} C:/libsodium-1.0.17/lib/libsodium.dll.a C:/libcurl-7.85/lib/libcurl.dll.a vertices)
elseif(UNIX AND NOT APPLE)
    # Find cURL
    find_package(CURL)
    include_directories(${CURL_INCLUDE_DIRS})
    include_directories(${INC_FOLDERS})
    add_executable(${PROJECT_NAME} ${SRC_FILES})
    target_link_libraries(${PROJECT_NAME} sodium curl vertices)
else()  # MacOS

endif()



