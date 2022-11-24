api_src = files(
  'database.cpp',
  'c_api.cpp',
)

api_lib = static_library(
  'api',
  api_src,
  dependencies: [ project_deps ],
  cpp_args: project_defs,
)

project_deps += declare_dependency(
  link_with: api_lib,
)
