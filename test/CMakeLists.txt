find_package(GTest REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wconversion -Wsign-conversion")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wfatal-errors")

include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/src/lib)
include_directories(${GTEST_INCLUDE_DIRS})

add_definitions(-DPROJECT_ROOT=\"${CMAKE_SOURCE_DIR}\")

add_subdirectory(parser)
add_subdirectory(rule)
add_subdirectory(pattern)
add_subdirectory(option)
add_subdirectory(rule_set)

add_executable(test_all
    reg_dom.cpp
    domain_data_base.cpp
    filter_rule_base.cpp
    element_hide_rule_base.cpp
    filter_set.cpp
    adblock.cpp
    api.cpp
)

target_link_libraries(test_all
    -Wl,--whole-archive
        parser_test
        rule_test
        pattern_test
        option_test
        rule_set_test
    -Wl,--no-whole-archive
    main
    adblock
    ${GTEST_BOTH_LIBRARIES}
)
