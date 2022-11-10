# SPDX-License-Identifier: Apache-2.0
message(STATUS "Vertices SDK location: ${CMAKE_CURRENT_SOURCE_DIR}")

set(VERTICES_SRC_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/account.c
        ${CMAKE_CURRENT_SOURCE_DIR}/utils/base64.c
        ${CMAKE_CURRENT_SOURCE_DIR}/utils/base32.c
        ${CMAKE_CURRENT_SOURCE_DIR}/utils/http_weak.c
        ${CMAKE_CURRENT_SOURCE_DIR}/utils/sha512_256.c
        ${CMAKE_CURRENT_SOURCE_DIR}/vertices_errors.c
        ${CMAKE_CURRENT_SOURCE_DIR}/vertices.c
        ${CMAKE_CURRENT_SOURCE_DIR}/algorand/algorand_parser.c
        ${CMAKE_CURRENT_SOURCE_DIR}/algorand/algorand_provider.c
        ${CMAKE_CURRENT_SOURCE_DIR}/algorand/algorand_transaction.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../external/mpack/src/mpack/mpack-common.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../external/mpack/src/mpack/mpack-reader.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../external/mpack/src/mpack/mpack-platform.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../external/mpack/src/mpack/mpack-expect.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../external/mpack/src/mpack/mpack-node.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../external/mpack/src/mpack/mpack-writer.c
        )

set(VERTICES_INC_DIRS
        "include"
        "../external/cJSON"
        "../external/mpack/src/mpack"
        )

string(LENGTH "${CMAKE_SOURCE_DIR}/" SOURCE_PATH_SIZE)



add_definitions("-DSOURCE_PATH_SIZE=${SOURCE_PATH_SIZE}")

if( DEFINED LIB_TYPE )
    add_library(vertices ${LIB_TYPE} ${VERTICES_SRC_FILES})
else()
    add_library(vertices STATIC ${VERTICES_SRC_FILES})
endif()

set_target_properties( vertices
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
        )

if( MSVC )

    #copy vertices library to unreal plugin
    add_custom_command(TARGET vertices  POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_BINARY_DIR}/bin
            ${CMAKE_BINARY_DIR}/../../UnrealAlgorandPlugin/Plugins/Algorand/Binaries/ThirdParty/AlgorandLibrary/Win64
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_BINARY_DIR}/lib
            ${CMAKE_BINARY_DIR}/../../UnrealAlgorandPlugin/Plugins/Algorand/Source/ThirdParty/AlgorandLibrary/x64/Release
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_BINARY_DIR}/bin
            ${CMAKE_BINARY_DIR}/../../UnrealAlgorandPlugin/Plugins/Algorand/Source/ThirdParty/AlgorandLibrary/x64/Release)

    # copy cjson library to unreal plugin
    add_custom_command(TARGET vertices  POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_BINARY_DIR}/external/cJSON/cjson.dll
            ${CMAKE_BINARY_DIR}/../../UnrealAlgorandPlugin/Plugins/Algorand/Binaries/ThirdParty/AlgorandLibrary/Win64
            COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_BINARY_DIR}/external/cJSON/cjson.dll
            ${CMAKE_BINARY_DIR}/../../UnrealAlgorandPlugin/Plugins/Algorand/Source/ThirdParty/AlgorandLibrary/x64/Release
            COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_BINARY_DIR}/external/cJSON/cjson.lib
            ${CMAKE_BINARY_DIR}/../../UnrealAlgorandPlugin/Plugins/Algorand/Source/ThirdParty/AlgorandLibrary/x64/Release)
endif()

target_include_directories(vertices
        PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/utils>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../external/cJSON>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../external/mbedtls/library>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../external/mbedtls/include>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../external/ed25519>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../external/mpack/src/mpack>"
        "$<INSTALL_INTERFACE:../include>"
        )

# public header files
install(FILES ../include/vertices.h ../include/vertices_errors.h ../include/vertices_types.h ../include/vertices_http.h DESTINATION include)

install(TARGETS vertices
        EXPORT verticesTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
        )


if(MSVC OR MSYS OR MINGW)
    target_link_libraries(vertices cjson mbedcrypto )
elseif(UNIX AND NOT APPLE)
    target_link_libraries(vertices cjson mbedcrypto)
#        target_compile_options(vertices PRIVATE -pedantic -Wall -Wstrict-prototypes -Wwrite-strings -Wshadow -Winit-self -Wformat=2 -Wmissing-prototypes -Wstrict-overflow=2 -Wc++-compat -Wundef -Wconversion -Wno-missing-braces)
else()  # MacOS

endif()
