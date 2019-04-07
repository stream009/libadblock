add_library(pattern_test STATIC
    basic_match_pattern.cpp
    domain_match_pattern.cpp
    regex_pattern.cpp
)
target_link_libraries(pattern_test
    pattern
    uri
)
