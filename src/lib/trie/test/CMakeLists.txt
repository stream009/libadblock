cmake_minimum_required(VERSION 3.4)
project(trie_test CXX)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

include_directories(..)

add_executable(test_main
    trie.cpp
    node.cpp
)
set_property(TARGET test_main PROPERTY CXX_STANDARD 11)
target_link_libraries(test_main
    ${GTEST_BOTH_LIBRARIES}
)

