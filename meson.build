project(
  'opengl_tester',
  'cpp',
  version: '0.1',
  default_options: ['warning_level=3', 'cpp_std=c++23'],
)

gl_dep = dependency('gl')
glew_dep = dependency('glew')
sdl2_dep = dependency('sdl2')
sdl2_image_dep = dependency('sdl2_image')
glm_dep = dependency('glm')

deps = [gl_dep, glew_dep, sdl2_dep, sdl2_image_dep, glm_dep]

inc = include_directories('include')

srcs = [
  'src/main.cpp',
  'src/gl_context.cpp',
  'src/shader.cpp',
  'src/asset_registry.cpp',
  'src/texture.cpp',
  'src/mesh.cpp',
  'src/mesh_loader.cpp',
  'src/input_manager.cpp',
  'src/camera.cpp',
]

exe = executable(
  'opengl_tester',
  srcs,
  include_directories: inc,
  dependencies: deps,
  install: true,
)

fs = import('fs')
fs.copyfile('shaders/vertex.vert.glsl')
fs.copyfile('shaders/fragment.frag.glsl')
fs.copyfile('imgs/test.png')
fs.copyfile('models/cube.obj')
fs.copyfile('models/sphere.obj')

test('basic', exe)
