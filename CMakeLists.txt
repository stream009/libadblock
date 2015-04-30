cmake_minimum_required(VERSION 3.1)

set(MAIN "wildcard")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/CMakePCHCompiler")

project(${MAIN} C CXX CXXPCH)

find_package(Boost 1.57 REQUIRED
	         COMPONENTS filesystem locale iostreams
                        program_options regex system timer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wfatal-errors")
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

add_library(rule OBJECT
    rule/filter_rule.cpp
    rule/element_hide_rule.cpp
    rule/basic_element_hide_rule.cpp
    rule/exception_element_hide_rule.cpp
    rule/comment_rule.cpp
)

add_library(rule_set OBJECT
    rule_set/substring_match_filter_rule_set.cpp
    rule_set/prefix_match_filter_rule_set.cpp
    rule_set/suffix_match_filter_rule_set.cpp
    rule_set/domain_match_filter_rule_set.cpp
    rule_set/domained_element_hide_rule_set.cpp
)

add_library(pattern OBJECT
    pattern/base_match_pattern.cpp
    pattern/basic_match_pattern.cpp
    pattern/domain_match_pattern.cpp
    pattern/regex_pattern.cpp
)

add_library(main OBJECT
    domain_data_base.cpp
    filter_rule_base.cpp
    element_hide_rule_base.cpp
    option.cpp
    subscription.cpp
    adblock.cpp
)

add_subdirectory(parser)
add_subdirectory(lib/reg_dom)

add_executable(${MAIN}
    main.cpp
    $<TARGET_OBJECTS:main>
    $<TARGET_OBJECTS:pattern>
    $<TARGET_OBJECTS:rule>
    $<TARGET_OBJECTS:rule_set>
    $<TARGET_OBJECTS:reg_dom>
)
target_link_libraries(${MAIN} cppnetlib-uri)
target_link_libraries(${MAIN} ${Boost_LIBRARIES})
target_link_libraries(${MAIN} parser)

add_subdirectory(test)

get_property(targets GLOBAL PROPERTY all_targets)
foreach(target_ ${targets})
    target_precompiled_header(${target_} stable.hpp REUSE global_pch)
endforeach()
