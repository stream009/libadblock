configure_file(PKGBUILD.in PKGBUILD @ONLY)

add_custom_target(arch_package
    makepkg -sc
)
