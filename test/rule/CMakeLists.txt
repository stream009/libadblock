add_library(rule_test STATIC
    filter_rule.cpp
    element_hide_rule.cpp
    comment_rule.cpp
)
target_link_libraries(rule_test
    rule
    uri
)
