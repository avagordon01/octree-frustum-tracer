project('octree-frustum-tracer', 'cpp',
  default_options: [
    'buildtype=debugoptimized',
    'warning_level=3',
    'werror=true',
    'cpp_std=c++17',
    'b_sanitize=address,undefined',
    'b_lundef=false',
  ]
)

eigen = dependency('eigen3')
openmesh = declare_dependency(link_args: '-lOpenMeshCore')

executable('screen-space', 'src/screen-space.cpp', dependencies: [eigen, openmesh])
executable('world-space', 'src/world-space.cpp', dependencies: [eigen, openmesh])
