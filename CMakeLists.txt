cmake_minimum_required(VERSION 3.14)

project(libadblock
    VERSION 1.1.3
    LANGUAGES C CXX
)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_subdirectory(src)
add_subdirectory(third_party/adblock_parser)
add_subdirectory(third_party/radix_tree)

if(BUILD_TESTING)
    add_subdirectory(test)
    add_subdirectory(benchmark)
endif()

add_subdirectory(arch)
