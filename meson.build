project('libadblock', 'cpp', 'c',
  default_options: ['cpp_std=c++20'], license: 'GPL3' )

# external dependencies for external and internal ones
# If dependency appear in public header, add it to the public_deps othewise
# private_deps will suffice.
public_deps = []
private_deps = [
  dependency('stream9_core'),
  dependency('adblock_parser'),
  dependency('stream9_radix_tree'),
  dependency('stream9_url'),
]

# include directories, external and internal ones
public_inc = include_directories('include')
private_inc = []

# library targets which user have to link with
project_libs = []

subdir('src')

# dependency for client to access with get_variable('project_dep')
project_dep = declare_dependency(
  include_directories: public_inc,
  dependencies: public_deps,
  link_with: project_libs,
)

impl_dep = declare_dependency(
  include_directories: src_inc,
  dependencies: [ private_deps,
    core_dep, rule_dep, index_dep, parser_dep, pattern_dep, reg_dom_dep, ]
)

# allow client to pull in library with dependency('PROJECT NAME')
meson.override_dependency(meson.project_name(), project_dep)
