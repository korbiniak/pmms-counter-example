project('counter_example', 'cpp')

srcdir = 'src'
testdir = 'test'

inc_dir = include_directories('src')

gtest_dep = dependency('gtest', required: true)
gmock_dep = dependency('gmock', required: true)

lib_src = files(
  srcdir / 'valuation.cpp',
  srcdir / 'pmms.cpp',
  srcdir / 'allocation.cpp',
)

test_src = files(
  testdir / 'test_pmms.cpp',
  testdir / 'test_main.cpp',
  testdir / 'test_bundle.cpp',
  testdir / 'test_valuation.cpp',
)

app_src = files(srcdir / 'main.cpp')

test_executable = executable('test_main', [test_src, lib_src], dependencies: [gtest_dep, gmock_dep], include_directories: inc_dir)

test('all_tests', test_executable)
executable('main', [lib_src, app_src], include_directories: inc_dir)
