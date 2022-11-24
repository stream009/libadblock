src_inc = include_directories('.', 'lib')

subdir('core')
subdir('rule')
subdir('index')
subdir('pattern')
subdir('parser')
subdir('lib/reg_dom')

project_src = files(
  'api/c_api.cpp',
  'api/database.cpp',
)

project_libs += shared_library(
  'adblock',
  project_src,
  include_directories: [ public_inc, private_inc ],
  dependencies: [ public_deps, private_deps,
    core_dep, rule_dep, index_dep, parser_dep, pattern_dep, reg_dom_dep,
  ],
  link_args: ['-Wl,--no-undefined', '-Wl,--exclude-libs=ALL'],
)
