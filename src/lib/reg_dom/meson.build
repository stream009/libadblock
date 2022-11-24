reg_dom_src = files(
  'regdom.c',
)

reg_dom_lib = static_library(
  'reg_dom',
  reg_dom_src,
  include_directories: [ public_inc, private_inc ],
  dependencies: [ public_deps, private_deps, ],
)

reg_dom_dep = declare_dependency(
  dependencies: [ public_deps ],
  link_with: reg_dom_lib,
)
