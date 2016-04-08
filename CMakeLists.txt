cmake_minimum_required(VERSION 3.1)

set(MAIN "libadblock")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/CMakePCHCompiler")

project(${MAIN} C CXX CXXPCH)

find_package(Boost 1.57 REQUIRED
	         COMPONENTS filesystem locale iostreams
                        program_options regex system timer)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARIES_DIRS})
link_libraries(${Boost_LIBRARIES} cppnetlib-uri)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC -fvisibility=hidden")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wfatal-errors")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -fvisibility=hidden")
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcolor-diagnostics")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DBOOST_SPIRIT_DEBUG)
endif()
include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/lib)

function(add_executable target_)
    _add_executable(${target_} ${ARGN})
    set_property(GLOBAL APPEND PROPERTY all_targets ${target_})
endfunction()

function(add_library target_)
    _add_library(${target_} ${ARGN})
    set_property(GLOBAL APPEND PROPERTY all_targets ${target_})
endfunction()

add_custom_target(global_pch)
target_precompiled_header(global_pch stable.hpp)

add_library(adblock SHARED
    api.cpp
)
target_link_libraries(adblock
    main
)
set_target_properties(adblock PROPERTIES
    VERSION 0.0.1
    SOVERSION 0
)
install(adblock)

add_library(main
    filter_rule_base.cpp
    element_hide_rule_base.cpp
    filter_set.cpp
    adblock.cpp
)
target_link_libraries(main
    rule_set
    parser
)

add_library(option
    option.cpp
    domain_data_base.cpp
)
target_link_libraries(option reg_dom)

add_subdirectory(rule)
add_subdirectory(rule_set)
add_subdirectory(pattern)
add_subdirectory(parser)
add_subdirectory(lib/reg_dom)

if(BUILD_TESTING)
    add_subdirectory(test)
endif()

get_property(targets GLOBAL PROPERTY all_targets)
foreach(target_ ${targets})
    target_precompiled_header(${target_} stable.hpp REUSE global_pch)
endforeach()
