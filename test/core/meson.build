core_src = files(
  'filter_rule_base.cpp',
  'element_hide_rule_base.cpp',
  'snippet_rule_base.cpp',
  'filter_list.cpp',
  'adblock.cpp',
)

core_lib = static_library(
  'core',
  core_src,
  dependencies: [ project_deps ],
  cpp_args: project_defs,
)

project_deps += declare_dependency(
  link_with: core_lib,
)
