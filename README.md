# Counter example for PMMS

Todo:

- [x] Implement allocation generation for given valuations.
- [x] Measure envy and search for allocation with minimal maximal envy.
- [x] Benchmark.
- [x] Implement random valuation generation.
- [x] Add multithreading for allocation searching.
- [x] Implement iteration over all allocations for any number of agents.
- [x] Check if valuations are MMS-feasible.
- [ ] Generate submodular valuations.
- [ ] Implement heuristic algorithms for finding an allocation.
- [ ] Implement mu function in pseudopolynomial time.
- [ ] Subcommands for better pipelining

## Building and running

You need `ninja`, `meson`, `gtest` and `gmock` installed (`libgtest-dev`
and `libgmock-dev` on Debian).

1.  Setup the build environment:

        meson setup build --buildtype=release
        meson setup debugbuild --buildtype=debug  # not required

2.  Building and testing:

        meson compile -C build
        meson test -C build

Binaries are in the build directory.

## Major improvements

- Preprocess valuation for every subset of items.
- Preprocess the mu function for every agent and subset of items.

Current benchmarks for 3 agents and {6..12} items:

        $ ./build/benchmark_main --benchmark_time_unit=ms
        2024-10-10T11:59:58+02:00
        Running ./build/benchmark_main
        Run on (16 X 4900 MHz CPU s)
        CPU Caches:
          L1 Data 48 KiB (x8)
          L1 Instruction 32 KiB (x8)
          L2 Unified 1280 KiB (x8)
          L3 Unified 20480 KiB (x1)
        Load Average: 1.86, 2.18, 4.74
        ***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
        -------------------------------------------------------------------------------------------------
        Benchmark                                                       Time             CPU   Iterations
        -------------------------------------------------------------------------------------------------
        BM_PmmsGetAllAllocations/6/min_time:2.000                   0.130 ms        0.130 ms        21344
        BM_PmmsGetAllAllocations/7/min_time:2.000                   0.651 ms        0.651 ms         4385
        BM_PmmsGetAllAllocations/8/min_time:2.000                    2.93 ms         2.93 ms          911
        BM_PmmsGetAllAllocations/9/min_time:2.000                    12.8 ms         12.8 ms          218
        BM_PmmsGetAllAllocations/10/min_time:2.000                   66.5 ms         66.5 ms           43
        BM_PmmsGetAllAllocations/11/min_time:2.000                    314 ms          314 ms            9
        BM_PmmsGetAllAllocations/12/min_time:2.000                   1433 ms         1433 ms            2
        BM_PmmsGetAllAllocationsPrecomputeMu/6/min_time:2.000       0.023 ms        0.023 ms       123722
        BM_PmmsGetAllAllocationsPrecomputeMu/7/min_time:2.000       0.076 ms        0.076 ms        36893
        BM_PmmsGetAllAllocationsPrecomputeMu/8/min_time:2.000       0.216 ms        0.216 ms        12906
        BM_PmmsGetAllAllocationsPrecomputeMu/9/min_time:2.000       0.557 ms        0.557 ms         5226
        BM_PmmsGetAllAllocationsPrecomputeMu/10/min_time:2.000       1.80 ms         1.80 ms         1550
        BM_PmmsGetAllAllocationsPrecomputeMu/11/min_time:2.000       5.80 ms         5.80 ms          484
        BM_PmmsGetAllAllocationsPrecomputeMu/12/min_time:2.000       14.8 ms         14.8 ms          189
        BM_iter_3/6/min_time:2.000                                   0.001 ms        0.001 ms      2717398
        BM_iter_3/7/min_time:2.000                                   0.003 ms        0.003 ms       932438
        BM_iter_3/8/min_time:2.000                                   0.009 ms        0.009 ms       312691
        BM_iter_3/9/min_time:2.000                                   0.027 ms        0.027 ms       106017
        BM_iter_3/10/min_time:2.000                                  0.079 ms        0.079 ms        35600
        BM_iter_3/11/min_time:2.000                                  0.230 ms        0.230 ms        12198
        BM_iter_3/12/min_time:2.000                                  0.673 ms        0.673 ms         4195
        BM_iter_n/6/min_time:2.000                                  0.001 ms        0.001 ms      2568918
        BM_iter_n/7/min_time:2.000                                  0.003 ms        0.003 ms       871657
        BM_iter_n/8/min_time:2.000                                  0.009 ms        0.009 ms       297980
        BM_iter_n/9/min_time:2.000                                  0.028 ms        0.028 ms        98870
        BM_iter_n/10/min_time:2.000                                 0.087 ms        0.087 ms        33443
        BM_iter_n/11/min_time:2.000                                 0.256 ms        0.256 ms        10708
        BM_iter_n/12/min_time:2.000                                 0.763 ms        0.763 ms         3658
