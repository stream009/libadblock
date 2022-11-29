parser_src = files(
  'rule_builder.cpp',
  'parser.cpp',
)

parser_lib = static_library(
  'parser',
  parser_src,
  include_directories: [ public_inc, private_inc ],
  dependencies: [ public_deps, private_deps, ],
)

parser_dep = declare_dependency(
  dependencies: [ public_deps ],
  link_with: parser_lib,
)
