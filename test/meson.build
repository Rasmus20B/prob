gtest_proj = subproject('gtest') 

gtest_dep = dependency('gtest', main : true, required : false) 

gtest_lib = library('gtest', install : true)

tests_src = [ 'main_test.cc', 'stack_test.cc' ]
e = executable('prob_tests',
                  sources : tests_src, 
                  dependencies : [ gtest_dep ],
                  link_with : [gtest_lib ])
test('gtest test', e)
