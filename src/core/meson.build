core_src = files(
  'file.cpp',
  'filter_rule_base.cpp',
  'element_hide_rule_base.cpp',
  'snippet_rule_base.cpp',
  'filter_list.cpp',
  'adblock.cpp',
)

core_lib = static_library(
  'core',
  core_src,
  include_directories: [ public_inc, private_inc, src_inc ],
  dependencies: [ public_deps, private_deps, ],
)

core_dep = declare_dependency(
  dependencies: [ public_deps ],
  link_with: core_lib,
)
