cmake_minimum_required(VERSION 3.14)

project(libadblock C CXX)

add_subdirectory(src)
add_subdirectory(third_party/adblock_parser)
add_subdirectory(third_party/radix_tree)

if(BUILD_TESTING)
    add_subdirectory(test)
    add_subdirectory(benchmark)
endif()
