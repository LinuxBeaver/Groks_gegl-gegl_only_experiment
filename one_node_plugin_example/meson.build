project('smooth', 'c',
  version: '0.1',
  license: 'GPL-3.0-or-later')

# Compiler arguments
lib_args = ['-DBUILDING_SMOOTH']

# Compiler
cc = meson.get_compiler('c')

# Dependencies
gegl_dep = dependency('gegl-0.4')
babl_dep = dependency('babl-0.1')
glib_dep = dependency('glib-2.0')
math_dep = cc.find_library('m', required: true)  # Link libm for fmaxf

# Build shared library
shared_library('smooth',
  sources: [
    'smooth.c',
    'gegl-buffer-cl-iterator.h',
    'gegl-buffer-cl-iterator.h',
    'config.h'
  ],
  c_args: lib_args,
  dependencies: [gegl_dep, babl_dep, glib_dep, math_dep],
  include_directories: include_directories('.'),
  name_prefix: '',
  install: true,
  install_dir: get_option('libdir') / 'gegl-0.4'
)
