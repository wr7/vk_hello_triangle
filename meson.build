project('vulkanhello', 'c',
  version : '0.1',
  default_options : ['warning_level=3'])

vulkan = dependency('vulkan')
glfw3 = dependency('glfw3')

exe = executable('vulkanhello', 'src/main.c',
  install : false, dependencies : [vulkan, glfw3])

test('basic', exe)
