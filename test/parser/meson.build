parser_src = files(
  'basic_match_pattern.cpp',
  'domain_match_pattern.cpp',
  'regex_pattern.cpp',
  'filter_option.cpp',
  'white_list_option.cpp',
  'domain_option.cpp',
  'site_key_option.cpp',
  'filter_rule.cpp',
  'element_hide_rule.cpp',
  'snippet_rule.cpp',
  'comment_rule.cpp',
  'parse_error.cpp',
)

parser_lib = static_library(
  'parser',
  parser_src,
  dependencies: [ project_deps ],
  cpp_args: project_defs,
)

project_deps += declare_dependency(
  link_with: parser_lib,
)
