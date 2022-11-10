set(TEST_SRC_EXTRA_SOURCES
        ../../vertices_ports/http_curl.c
        ../../fakes/fake_vertices_errors.c
        ../../src/AllTests.cpp
        )

add_executable(${TEST_PROJECT_NAME} EXCLUDE_FROM_ALL ${TEST_SRC_SOURCES} ${TEST_SRC_EXTRA_SOURCES})
target_link_libraries(${TEST_PROJECT_NAME} sodium curl vertices ${CPPUTEST_LDFLAGS})


