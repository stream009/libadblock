set(SRCS
    reg_dom.cpp
    domain_data_base.cpp
    filter_rule_base.cpp
    element_hide_rule_base.cpp
    subscription.cpp
    adblock.cpp
    misc.cpp
    pattern/basic_match_pattern.cpp
    pattern/domain_match_pattern.cpp
    pattern/regex_pattern.cpp
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
    rule/filter_rule.cpp
    rule/element_hide_rule.cpp
    rule/comment_rule.cpp
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
    rule_set/substring_match_filter_rule_set.cpp
    rule_set/prefix_match_filter_rule_set.cpp
    rule_set/suffix_match_filter_rule_set.cpp
    rule_set/domain_match_filter_rule_set.cpp
    rule_set/domained_element_hide_rule_set.cpp
    $<TARGET_OBJECTS:main>
    $<TARGET_OBJECTS:pattern>
    $<TARGET_OBJECTS:rule>
    $<TARGET_OBJECTS:rule_set>
    $<TARGET_OBJECTS:reg_dom>
)
set(TARGET test_main)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_definitions(-DPROJECT_ROOT=\"${CMAKE_SOURCE_DIR}\")

add_executable(${TARGET} ${SRCS})
target_link_libraries(${TARGET} ${GTEST_BOTH_LIBRARIES})
target_link_libraries(${TARGET} cppnetlib-uri)
target_link_libraries(${TARGET} ${Boost_LIBRARIES})
target_link_libraries(${TARGET} parser)

add_executable(prefix_map
    prefix_map.cpp
)
target_link_libraries(prefix_map ${GTEST_BOTH_LIBRARIES})
target_link_libraries(prefix_map ${Boost_LIBRARIES})

add_executable(trie
    trie/trie.cpp
    trie/node.cpp
)
target_link_libraries(trie ${GTEST_BOTH_LIBRARIES})
target_link_libraries(trie ${Boost_LIBRARIES})
