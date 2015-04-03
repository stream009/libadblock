cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_COMPILER clang++)
set(MAIN "wildcard")

project(${MAIN} CXX)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wfatal-errors")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcolor-diagnostics")

find_package(Boost 1.57 REQUIRED COMPONENTS regex system)

set(SRCS main.cpp 
	 rule/filter_rule.cpp
	 rule/element_hide_rule.cpp
	 rule/basic_element_hide_rule.cpp
	 rule/exception_element_hide_rule.cpp
	 rule/comment_rule.cpp
         pattern/basic_match_pattern.cpp
	 pattern/begin_match_pattern.cpp
	 pattern/end_match_pattern.cpp
	 pattern/domain_match_pattern.cpp
	 pattern/regex_pattern.cpp
	 parser/parser.cpp
	 parser/domain.cpp
	 parser/filter_rule.cpp
	 parser/element_hide_rule.cpp
	 parser/grammar.cpp
)

add_executable(${MAIN} ${SRCS})

include_directories(${CMAKE_SOURCE_DIR})
target_link_libraries(${MAIN} cppnetlib-uri)
target_link_libraries(${MAIN} ${Boost_LIBRARIES})
add_definitions(-DBOOST_SPIRIT_DEBUG)

include(PrecompiledHeader)
add_precompiled_header(${MAIN} stable.hpp FORCEINCLUDE)
