rule_src = files(
  'filter_rule.cpp',
  'element_hide_rule.cpp',
  'snippet_rule.cpp',
  'comment_rule.cpp',
)

rule_lib = static_library(
  'rule',
  rule_src,
  dependencies: [ project_deps ],
  cpp_args: project_defs,
)

project_deps += declare_dependency(
  dependencies: [],
  link_with: rule_lib,
)
