pattern_src = files(
  'basic_match_pattern.cpp',
  'domain_match_pattern.cpp',
  'regex_pattern.cpp',
)

pattern_lib = static_library(
  'pattern',
  pattern_src,
  dependencies: [ project_deps ],
  cpp_args: project_defs,
)

project_deps += declare_dependency(
  dependencies: [],
  link_with: pattern_lib,
)
