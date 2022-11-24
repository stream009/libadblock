index_src = files(
  'substring_match_filter_rule_map.cpp',
  'prefix_match_filter_rule_map.cpp',
  'suffix_match_filter_rule_map.cpp',
  'domain_match_filter_rule_map.cpp',
  'domained_element_hide_rule_map.cpp',
)

index_lib = static_library(
  'index',
  index_src,
  dependencies: [ project_deps ],
  cpp_args: project_defs,
)

project_deps += declare_dependency(
  dependencies: [],
  link_with: index_lib,
)
