project('vulkanhello', 'c',
  version : '0.1',
  default_options : ['warning_level=3'])

vulkan = dependency('vulkan')
glfw3 = dependency('glfw3')

project_source_files = [
  'src/main.c',
  'src/util.c',
  'src/vulkan_setup.c',
  'src/vulkan_setup/device_setup.c',
  'src/main_window.c'
]

include_directories = include_directories('src')

exe = executable('vulkanhello', project_source_files,
  install : false, include_directories : include_directories, dependencies : [vulkan, glfw3])

test('basic', exe)
