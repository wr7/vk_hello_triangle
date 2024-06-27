project('vulkanhello', 'c',
  version : '0.1',
  default_options : ['warning_level=3', 'c_std=c17', 'optimization=1'])

cc = meson.get_compiler('c')

# Dependencies

vulkan = dependency('vulkan')

if build_machine.system() != 'windows'
	glfw3 = dependency('glfw3')
else
	glfw3_inc = include_directories('windeps/glfw/include')
	glfw3_lib = cc.find_library('glfw3', dirs: [meson.current_source_dir() + '/windeps/glfw/'])
	glfw3 = declare_dependency(dependencies: [glfw3_lib], include_directories: glfw3_inc)
endif

libm = cc.find_library('m', required : false)

runtime_deps = [vulkan, glfw3, libm]

ffmpeg_deps = [
  dependency('libswscale'),
  dependency('libavutil'),
  dependency('libavformat'),
  dependency('libavcodec'),
]

libbce = declare_dependency(
  link_with: [static_library('libbce', 'deps/libbce/libbce.c')],
  include_directories: ['deps/libbce'])

# Compile time processing programs

glslc_prog = find_program('glslc')
bce_prog = executable('bce', 'deps/bce.c', install: false)
image_decoder_prog = executable('image_decoder', 'comptime/image_decoder.c', 
                                dependencies: ffmpeg_deps + libbce, install: false)

glslc = generator(glslc_prog,
  output  : '@PLAINNAME@.spv',
  arguments : ['@INPUT@', '-o', '@OUTPUT@'])

bce = generator(bce_prog,
  output  : '@BASENAME@.h',
  arguments : ['@INPUT@', '@OUTPUT@'])

image_decoder = generator(image_decoder_prog,
  output  : '@PLAINNAME@.h',
  arguments : ['@INPUT@', '@OUTPUT@'])

# Sources

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
  install : false, include_directories : include_directories, dependencies : runtime_deps)
