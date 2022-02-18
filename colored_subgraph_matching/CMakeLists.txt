add_executable(colored_subgraph_matcher colored_subgraph_matcher_main.cpp colored_graph.cpp colored_graph_subgraph_matching.cpp colored_graph_subgraph_matching.h)

find_package(GTest REQUIRED)
enable_testing()
SET(TEST_EXE_NAME colored_graph_unit_tests)
SET(TEST_SOURCE_FILES
        colored_graph.cpp
        colored_graph_subgraph_matching.cpp
        colored_graph_unit_testing.cpp
)

add_executable(${TEST_EXE_NAME} ${TEST_SOURCE_FILES})

target_link_libraries(${TEST_EXE_NAME} PUBLIC
        GTest::GTest
        )

include(GoogleTest)
gtest_discover_tests(${TEST_EXE_NAME})