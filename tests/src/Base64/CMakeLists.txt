cmake_minimum_required( VERSION 3.5 )

set(TEST_PROJECT_NAME Base64)
project(${TEST_PROJECT_NAME} C CXX)

set( TEST_SRC_SOURCES
        test_base64.cpp
        )

include( ${VERTICES_TEST_ROOT}/GenerateTest.cmake )