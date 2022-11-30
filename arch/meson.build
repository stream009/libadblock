conf_data = configuration_data()
conf_data.set('version', meson.project_version())
configure_file(
  input: 'PKGBUILD.in',
  output: 'PKGBUILD',
  configuration: conf_data,
)
