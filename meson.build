project('vulkanhello', 'c',
  version : '0.1',
  default_options : ['warning_level=3', 'c_std=c17', 'optimization=1'])

cc = meson.get_compiler('c')

glslc_prog = find_program('glslc')
bce_prog = executable('bce', 'bce.c')

glslc = generator(glslc_prog,
  output  : '@PLAINNAME@.spv',
  arguments : ['@INPUT@', '-o', '@OUTPUT@'])

bce = generator(bce_prog,
  output  : '@BASENAME@.h',
  arguments : ['@INPUT@', '@OUTPUT@'])

vulkan = dependency('vulkan')
glfw3 = dependency('glfw3')
libm = cc.find_library('m', required : false)

shader_source_files = [
  'src/shader.vert',
  'src/shader.frag'
]

project_source_files = [
  'src/draw.c',
  'src/main.c',
  'src/main_window.c',
  'src/math_types.c',
  'src/util.c',
  'src/vulkan_setup.c',
  'src/vulkan_setup/command_buffer.c',
  'src/vulkan_setup/descriptor_sets.c',
  'src/vulkan_setup/device.c',
  'src/vulkan_setup/framebuffers.c',
  'src/vulkan_setup/graphics_card.c',
  'src/vulkan_setup/images.c',
  'src/vulkan_setup/pipeline.c',
  'src/vulkan_setup/queues.c',
  'src/vulkan_setup/render_pass.c',
  'src/vulkan_setup/swapchain.c',
  'src/vulkan_setup/synchronization.c',
  'src/vulkan_setup/buffers.c'
]

compiled_shaders = glslc.process(shader_source_files)
shader_headers = bce.process(compiled_shaders)

include_directories = include_directories('src')

exe = executable('vulkanhello', project_source_files + shader_headers,
  install : false, include_directories : include_directories, dependencies : [vulkan, glfw3, libm])
