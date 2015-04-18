set(SRCS
    parser.cpp
    domain.cpp
    filter_rule.cpp
    element_hide_rule.cpp
    grammar.cpp
    filter_pattern.cpp
    filter_option.cpp
    white_list_option.cpp
)

set(TARGET parser)

add_library(${TARGET} STATIC ${SRCS})
