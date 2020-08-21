project('octree-frustum-tracer', 'cpp',
  default_options: [
    'prefix=' + meson.source_root() / 'packaged',
    'buildtype=debugoptimized',
    'warning_level=3',
    'werror=true',
    'cpp_std=c++17',
    'b_sanitize=address,undefined',
    'b_lundef=false',
  ]
)

add_project_arguments(
  '-Wno-deprecated-copy',
  '-Wno-unused-parameter',
  language: 'cpp',
)

openmesh = declare_dependency(link_args: '-lOpenMeshCore')
eigen = dependency('eigen3')
glfw = dependency('glfw3')
glew = dependency('glew')

executable('screen-space', 'src/screen-space.cpp', 'src/controller.cc', 'src/renderer.cc', dependencies: [eigen, openmesh, glfw, glew], install: true)
executable('world-space', 'src/world-space.cpp', 'src/controller.cc', 'src/renderer.cc', dependencies: [eigen, openmesh, glfw, glew], install: true)

executable('renderer', 'src/main.cc', 'src/controller.cc', 'src/renderer.cc', dependencies: [eigen, glfw, glew], install: true)
