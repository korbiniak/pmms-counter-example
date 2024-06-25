project(
  'counter_example',
  'cpp',
  default_options: [
    'cpp_std=c++17',
    'b_ndebug=if-release',
    'werror=true',
    'warning_level=2',
  ],
)

cmake = import('cmake')
opt_var = cmake.subproject_options()
opt_var.add_cmake_defines({'BENCHMARK_ENABLE_GTEST_TESTS': 'OFF'})
libgooglebench = cmake.subproject('libgoogle_benchmark', options: opt_var)

add_project_arguments(
  [
    '-Wshadow',
  ],
  language: 'cpp',
)

if get_option('buildtype') == 'debug'
  add_project_arguments(
    [
      '-Og',
    ],
    language: 'cpp'
  )
elif get_option('buildtype') == 'release'
  add_project_arguments(
     [
     ],
     language: 'cpp'
   )
endif

profiling = get_option('profiling')
if profiling
  add_project_arguments('-pg', language: 'cpp')
  add_project_link_arguments('-pg', language: 'cpp')
endif

srcdir = 'src'
testdir = 'test'
benchmarkdir = 'benchmark'

inc_dir = include_directories('src')

gtest_dep = dependency('gtest', required: true)
gmock_dep = dependency('gmock', required: true)

lib_src = files(
  srcdir / 'valuation.cpp',
  srcdir / 'pmms.cpp',
  srcdir / 'efx.cpp',
  srcdir / 'allocation.cpp',
  srcdir / 'generator.cpp',
  srcdir / 'simulated_annealing.cpp',
)

test_src = files(
  testdir / 'test_common.cpp',
  testdir / 'test_pmms.cpp',
  testdir / 'test_main.cpp',
  testdir / 'test_bundle.cpp',
  testdir / 'test_valuation.cpp',
  testdir / 'test_allocation.cpp',
  testdir / 'test_generator.cpp',
  testdir / 'test_efx.cpp',
)

benchmark_src = files(
  benchmarkdir / 'benchmark_pmms.cpp'
)

random_search_src = files(srcdir / 'random_search_main.cpp')
simulated_annealing_src = files(srcdir / 'simulated_annealing_main.cpp')

test_executable = executable(
  'test_main',
  [test_src, lib_src],
  dependencies: [gtest_dep, gmock_dep],
  include_directories: inc_dir
)


test('all_tests', test_executable, workdir : meson.source_root())
executable('random_search', [lib_src, random_search_src], include_directories: inc_dir)
executable('random_search_int', [lib_src, random_search_src], include_directories: inc_dir, cpp_args: '-DVALUATION_T_INT64')

executable('simulated_annealing', [lib_src, simulated_annealing_src], include_directories: inc_dir)
executable('simulated_annealing_int', [lib_src, simulated_annealing_src], include_directories: inc_dir, cpp_args: '-DVALUATION_T_INT64')

executable(
  'benchmark_main',
  [lib_src, benchmark_src],
  include_directories: inc_dir,
  dependencies: [ libgooglebench.dependency('benchmark') ]
)
