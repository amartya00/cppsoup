project(
  'cppsoup',
  'cpp',
  version : '0.0.1',
  license : 'GPLV2',
  default_options:[
    'warning_level=3'
  ])

add_global_arguments('--std=c++17', '-Werror', language : 'cpp')

#Dependencies
catch = dependency('catch2')

inc = include_directories('headers')

subdir('headers')
subdir('tst')