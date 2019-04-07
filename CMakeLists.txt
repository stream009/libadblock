cmake_minimum_required(VERSION 3.1)

project(libadblock C CXX)

add_subdirectory(src)
add_subdirectory(third_party/adblock_parser)

if(BUILD_TESTING)
    add_subdirectory(test)
endif()
