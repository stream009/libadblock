add_library(parser_test STATIC
    basic_match_pattern.cpp
    domain_match_pattern.cpp
    regex_pattern.cpp
    filter_option.cpp
    white_list_option.cpp
    domain_option.cpp
    site_key_option.cpp
    filter_rule.cpp
    element_hide_rule.cpp
    comment_rule.cpp
)
target_link_libraries(parser_test
    parser
)
