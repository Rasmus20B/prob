project('prob', 'cpp', version: '0.0.1',
  license: 'MIT',
  default_options: [
    'buildtype=release',
    'warning_level=3',
    'cpp_std=c++20'
  ]
)

lodge_dep = dependency('lodge', fallback: 'lodge')

prob = executable('prob', sources : [ 'src/main.cc', 'src/prob.cc', 'src/parse.cc', 'src/ast.cc', 'src/stack.h', 'src/ast.h', 'src/prob.h', 'src/p_val.h', 'src/parse.h', 'src/tac.h', 'src/token.h' ], dependencies : [ lodge_dep ] )

build_tests = get_option('build_tests')
if build_tests
subdir('test')
endif  
