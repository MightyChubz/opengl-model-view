project(
  'opengl_tester',
  'cpp',
  version: '0.1',
  default_options: ['warning_level=3', 'cpp_std=c++23'],
)

gl_dep = dependency('gl')
glew_dep = dependency('glew', version: '2.2.0')
sdl2_dep = dependency('sdl2', version: '2.28.3')
sdl2_image_dep = dependency('sdl2_image', version: '2.8.2')
glm_dep = dependency('glm', version: 'cci.20230113')

deps = [gl_dep, glew_dep, sdl2_dep, sdl2_image_dep, glm_dep]

inc = include_directories('include')

srcs = [
  'src/main.cpp',
  'src/SdlSubsystem.cpp',
  'src/GLContext.cpp',
  'src/GameWindow.cpp',
  'src/Shader.cpp',
  'src/AssetRegistry.cpp',
  'src/Texture.cpp',
  'src/Mesh.cpp',
  'src/Model.cpp',
  'src/MeshLoader.cpp',
  'src/InputManager.cpp',
  'src/Camera.cpp',
  'src/MaterialSystem.cpp',
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

test('basic', exe, timeout: 9999)
