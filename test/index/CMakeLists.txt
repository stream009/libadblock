add_library(index_test STATIC
    substring_match_filter_rule_map.cpp
    prefix_match_filter_rule_map.cpp
    suffix_match_filter_rule_map.cpp
    domain_match_filter_rule_map.cpp
    domained_element_hide_rule_map.cpp
)
target_link_libraries(index_test
    index
    core
)
