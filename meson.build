project('prob', 'cpp', version: '0.0.1',
  license: 'MIT',
  default_options: [
    'buildtype=release',
    'warning_level=3',
    'cpp_std=c++20'
  ]
)

lodge_dep = dependency('lodge', fallback: 'lodge')
# lodge_proj = subproject('lodge')
# lodge_dep = lodge_proj.get_variable('lodge_dep')

prob = executable('prob', sources : [ 'src/main.cc', 'src/prob.cc', 'src/parse.cc', 'src/prob.h', 'src/p_val.h', 'src/parse.h', 'src/tac.h' ], dependencies : [ lodge_dep ] )

