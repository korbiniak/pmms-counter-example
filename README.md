# Counter example for PMMS

Todo:

- [x] Implement allocation generation for given valuations.
- [ ] Measure envy and search for allocation with maximal minimal envy.
- [ ] Implement random valuation generation.
- [ ] Benchmark.
- [ ] Implement heuristic algorithms for finding an allocation.
- [ ] Implement mu function in pseudopolynomial time.

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
