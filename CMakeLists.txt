cmake_minimum_required(VERSION 3.14)

project(libadblock
    VERSION 2.0.1
    LANGUAGES C CXX
)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(boost_dir "${CMAKE_CURRENT_SOURCE_DIR}/third_party/boost_1_69_0")
include_directories(SYSTEM ${boost_dir})
set(boost_lib_dir ${boost_dir}/stage/lib)
link_directories(boost_lib_dir)
set(boost_libs "${boost_lib_dir}/libboost_regex.a")

add_subdirectory(third_party/adblock_parser)
add_subdirectory(third_party/radix_tree)
add_subdirectory(third_party/url)
add_subdirectory(src)

if(BUILD_TESTING)
    add_subdirectory(test)
    add_subdirectory(benchmark)
endif()

add_subdirectory(arch)
