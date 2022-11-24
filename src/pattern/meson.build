pattern_src = files(
  'base_match_pattern.cpp',
  'basic_match_pattern.cpp',
  'domain_match_pattern.cpp',
  'regex_pattern.cpp',
)

pattern_lib = static_library(
  'pattern',
  pattern_src,
  include_directories: [ public_inc, private_inc, src_inc, ],
  dependencies: [ public_deps, private_deps, ],
)

pattern_dep = declare_dependency(
  dependencies: [ public_deps ],
  link_with: pattern_lib,
)
