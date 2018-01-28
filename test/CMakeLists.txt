find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_definitions(-DPROJECT_ROOT=\"${CMAKE_SOURCE_DIR}\")

link_libraries(
    ${GTEST_BOTH_LIBRARIES}
)

enable_testing()

add_executable(test_main
    reg_dom.cpp
    domain_data_base.cpp
    filter_rule_base.cpp
    element_hide_rule_base.cpp
    filter_set.cpp
    adblock.cpp
    misc.cpp
)
target_link_libraries(test_main
    main
)
add_test(main test_main)

add_executable(test_rule_set
    rule_set/substring_match_filter_rule_set.cpp
    rule_set/prefix_match_filter_rule_set.cpp
    rule_set/suffix_match_filter_rule_set.cpp
    rule_set/domain_match_filter_rule_set.cpp
    rule_set/domained_element_hide_rule_set.cpp
)
target_link_libraries(test_rule_set
    rule_set
    uri
)
add_test(rule_set test_rule_set)

add_executable(test_rule
    rule/filter_rule.cpp
    rule/element_hide_rule.cpp
    rule/comment_rule.cpp
)
target_link_libraries(test_rule
    rule
    uri
)
add_test(rule test_rule)

add_executable(test_pattern
    pattern/basic_match_pattern.cpp
    pattern/domain_match_pattern.cpp
    pattern/regex_pattern.cpp
)
target_link_libraries(test_pattern
    pattern
    uri
)
add_test(pattern test_pattern)

add_executable(test_parser
    parser/domain.cpp
    parser/regex_pattern.cpp
    parser/basic_match_pattern.cpp
    parser/domain_match_pattern.cpp
    parser/filter_option.cpp
    parser/domain_option.cpp
    parser/sitekey_option.cpp
    parser/filter_rule.cpp
    parser/element_hide_rule.cpp
    parser/comment_rule.cpp
    parser/white_list_option.cpp
    parser/header.cpp
    parser/parser.cpp
)
target_link_libraries(test_parser
    parser
    uri
)
add_test(parser test_parser)

add_executable(test_option
    option/script.cpp
    option/image.cpp
    option/stylesheet.cpp
    option/object.cpp
    option/xmlhttprequest.cpp
    option/object_sub_request.cpp
    option/sub_document.cpp
    option/media.cpp
    option/other.cpp
    option/third_party.cpp
    option/domain.cpp
    option/site_key.cpp
    option/popup.cpp
    option/generic_hide.cpp
    option/generic_block.cpp
    option/websocket.cpp
    option/document.cpp
    option/elem_hide.cpp
    option/ping.cpp
)
target_link_libraries(test_option
    reg_dom
    main
)
add_test(option test_option)

add_executable(test_api
    api.cpp
)
target_link_libraries(test_api
    adblock
)
add_test(api test_api)
