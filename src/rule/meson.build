rule_src = files(
  'rule.cpp',
  'filter_rule.cpp',
  'element_hide_rule.cpp',
  'basic_element_hide_rule.cpp',
  'exception_element_hide_rule.cpp',
  'extended_element_hide_rule.cpp',
  'snippet_rule.cpp',
  'comment_rule.cpp',
  'filter_option.cpp',
  'domain_data_base.cpp',
  'utility.cpp',
)

rule_lib = static_library(
  'rule',
  rule_src,
  include_directories: [ public_inc, private_inc ],
  dependencies: [ public_deps, private_deps, ],
)

rule_dep = declare_dependency(
  dependencies: [ public_deps ],
  link_with: rule_lib,
)
