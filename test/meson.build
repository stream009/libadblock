project('libadblock_test', 'cpp',
  default_options: [
    'cpp_std=c++20',
    'warning_level=3',
    'b_sanitize=address,undefined'],
  license: 'GPL3' )

add_project_arguments(['-Wconversion', '-Wsign-conversion', '-fmax-errors=5'], language: 'cpp')

libadblock = subproject('libadblock')

project_deps = [
  dependency('gtest'),
  dependency('gtest_main'),
  libadblock.get_variable('project_dep'),
  libadblock.get_variable('impl_dep'),
  dependency('stream9_url'),
]

project_inc = []

project_root = meson.current_source_dir() / '..'

project_defs = [
  '-DDATA_DIR=' + meson.current_source_dir() / 'data',
  '-DPROJECT_ROOT=' + '"' + project_root + '"',
]

subdir('api')
subdir('core')
subdir('parser')
subdir('rule')
subdir('pattern')
subdir('option')
subdir('index')

test_src = files(
  'reg_dom.cpp',
  'domain_data_base.cpp',
)

test_exe = executable(
  meson.project_name(),
  test_src,
  include_directories: project_inc,
  dependencies: project_deps,
  cpp_args: project_defs,
  link_whole: [
    api_lib, core_lib, parser_lib, rule_lib,
    pattern_lib, option_lib, index_lib,
  ],
)

run_target('run',
  command: [test_exe, '--gtest_brief=1'],
)
