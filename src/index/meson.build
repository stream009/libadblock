index_src = files(
  'substring_match_filter_rule_map.cpp',
  'prefix_match_filter_rule_map.cpp',
  'suffix_match_filter_rule_map.cpp',
  'domain_match_filter_rule_map.cpp',
  'domained_element_hide_rule_map.cpp',
  'filter_rule_index.cpp',
)

index_lib = static_library(
  'index',
  index_src,
  include_directories: [ public_inc, private_inc, src_inc ],
  dependencies: [ public_deps, private_deps, ],
)

index_dep = declare_dependency(
  dependencies: [ public_deps, ],
  link_with: index_lib,
)
