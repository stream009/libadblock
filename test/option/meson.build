option_src = files(
  'script.cpp',
  'image.cpp',
  'stylesheet.cpp',
  'object.cpp',
  'xmlhttprequest.cpp',
  'object_sub_request.cpp',
  'sub_document.cpp',
  'media.cpp',
  'font.cpp',
  'other.cpp',
  'third_party.cpp',
  'domain.cpp',
  'site_key.cpp',
  'popup.cpp',
  'generic_hide.cpp',
  'generic_block.cpp',
  'websocket.cpp',
  'document.cpp',
  'elem_hide.cpp',
  'ping.cpp',
  'webrtc.cpp',
  'csp.cpp',
)

option_lib = static_library(
  'option',
  option_src,
  dependencies: [ project_deps ],
  cpp_args: project_defs,
)

project_deps += declare_dependency(
  dependencies: [],
  link_with: option_lib,
)
