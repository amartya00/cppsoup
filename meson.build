project(
  'cppsoup',
  'cpp',
  version : '0.0.1',
  license : 'GPLV2',
  default_options:[
    'warning_level=3',
    'werror=True'
  ])

add_global_arguments('--std=c++17', language : 'cpp')

#Dependencies
catch = dependency('catch2-with-main')

# Sources
inc = include_directories('headers')
subdir('headers')
subdir('tst')

# Set up pkgconfig
pkg = import('pkgconfig')
pkg.generate(
    subdirs : 'headers/thesoup',
    version : '0.0.2',
    name : 'thesoup',
    filebase : 'thesoup',
    description : 'A variety of c++ utilities.'
)