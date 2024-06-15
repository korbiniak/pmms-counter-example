# Counter example for PMMS

Todo:

- [x] Implement allocation generation for given valuations.
- [x] Measure envy and search for allocation with minimal maximal envy.
- [x] Benchmark.
- [x] Implement random valuation generation.
- [x] Add multithreading for allocation searching.
- [ ] Implement heuristic algorithms for finding an allocation.
- [ ] Implement mu function in pseudopolynomial time.
- [ ] Implement iteration over all allocations for any number of agents.

## Building and running

You need `meson`, `gtest` and `gmock` installed (`libgtest-dev`
and `libgmock-dev` on Debian).

1. Setup the build environment:

        meson setup build --buildtype=release
        meson setup debugbuild --buildtype=debug  # not required

2. Building and testing:

        meson compile -C build
        meson test -C build

Binaries are in the build directory.

## Major improvements

- Preprocess valuation for every subset of items.
- Preprocess the mu function for every agent and subset of items.

Currenct benchmarks for 3 agents and {6..12} items:

-------------------------------------------------------------------------------------------------
Benchmark                                                       Time             CPU   Iterations
-------------------------------------------------------------------------------------------------
BM_PmmsGetAllAllocations/6/min_time:2.000                   0.054 ms        0.054 ms        51389
BM_PmmsGetAllAllocations/7/min_time:2.000                   0.243 ms        0.243 ms        11557
BM_PmmsGetAllAllocations/8/min_time:2.000                    1.15 ms         1.15 ms         2450
BM_PmmsGetAllAllocations/9/min_time:2.000                    5.22 ms         5.22 ms          547
BM_PmmsGetAllAllocations/10/min_time:2.000                   25.0 ms         25.0 ms          109
BM_PmmsGetAllAllocations/11/min_time:2.000                    117 ms          117 ms           24
BM_PmmsGetAllAllocations/12/min_time:2.000                    539 ms          539 ms            5
BM_PmmsGetAllAllocationsPrecomputeMu/6/min_time:2.000       0.025 ms        0.025 ms       112148
BM_PmmsGetAllAllocationsPrecomputeMu/7/min_time:2.000       0.081 ms        0.081 ms        34522
BM_PmmsGetAllAllocationsPrecomputeMu/8/min_time:2.000       0.245 ms        0.245 ms        11456
BM_PmmsGetAllAllocationsPrecomputeMu/9/min_time:2.000       0.667 ms        0.667 ms         4192
BM_PmmsGetAllAllocationsPrecomputeMu/10/min_time:2.000       2.18 ms         2.18 ms         1279
BM_PmmsGetAllAllocationsPrecomputeMu/11/min_time:2.000       7.15 ms         7.15 ms          392
BM_PmmsGetAllAllocationsPrecomputeMu/12/min_time:2.000       18.7 ms         18.7 ms          149
