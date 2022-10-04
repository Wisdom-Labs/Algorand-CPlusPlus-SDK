
add_executable(${TEST_PROJECT_NAME} EXCLUDE_FROM_ALL ${TEST_SRC_SOURCES} ${VERTICES_TEST_ROOT}/src/AllTests.cpp)
target_link_libraries(${TEST_PROJECT_NAME} sodium curl vertices ${CPPUTEST_LDFLAGS})