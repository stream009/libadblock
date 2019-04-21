add_library(core_test STATIC
    filter_rule_base.cpp
    element_hide_rule_base.cpp
    filter_list.cpp
    adblock.cpp
)
target_link_libraries(core_test
    core
)
