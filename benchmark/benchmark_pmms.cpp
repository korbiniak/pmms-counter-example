#include <benchmark/benchmark.h>
#include "src/allocation.h"
#include "src/pmms.h"
#include "src/valuation.h"

static void BM_PmmsGetAllAllocations(benchmark::State& state) {
  int range = state.range(0);

  Valuation valuation1 =
      Valuation(std::vector<valuation_t>(range, 1)).normalize();
  Valuation valuation2 =
      Valuation(std::vector<valuation_t>(range, 1)).normalize();
  Valuation valuation3 =
      Valuation(std::vector<valuation_t>(range, 1)).normalize();
  for (auto _ : state) {
    benchmark::DoNotOptimize(
        Pmms::getAllAllocations({valuation1, valuation2, valuation3}));
  }
}

static void BM_PmmsGetAllAllocationsPrecomputeMu(benchmark::State& state) {
  int range = state.range(0);

  Valuation valuation1 =
      Valuation(std::vector<valuation_t>(range, 1)).normalize();
  Valuation valuation2 =
      Valuation(std::vector<valuation_t>(range, 1)).normalize();
  Valuation valuation3 =
      Valuation(std::vector<valuation_t>(range, 1)).normalize();

  for (auto _ : state) {
    benchmark::DoNotOptimize(Pmms::getAllAllocationsPrecomputeMu(
        {valuation1, valuation2, valuation3}));
  }
}

static void BM_iter_n(benchmark::State& state) {
  int range = state.range(0);

  for (auto _ : state) {
    Allocation::iter_n(3, range, [&](const Allocation& allocation) {
      benchmark::DoNotOptimize(allocation.agents());
      return true;
    });
  }
}

static void BM_iter_3(benchmark::State& state) {
  int range = state.range(0);

  for (auto _ : state) {
    Allocation::iter_3(range, [&](const Allocation& allocation) {
      benchmark::DoNotOptimize(allocation.agents());
      return true;
    });
  }
}

static void BM_IsMmsFeasible(benchmark::State& state) {
  int m = state.range(0);

  // Create a valuation with m items, all with value 1
  std::vector<valuation_t> values(m, 1);
  Valuation valuation(values);

  for (auto _ : state) {
    benchmark::DoNotOptimize(Pmms::isMmsFeasible(valuation));
  }
}

BENCHMARK(BM_PmmsGetAllAllocations)->DenseRange(6, 12)->MinTime(2.0);
BENCHMARK(BM_PmmsGetAllAllocationsPrecomputeMu)
    ->DenseRange(6, 12)
    ->MinTime(2.0);

BENCHMARK(BM_iter_3)->DenseRange(6, 12)->MinTime(2.0);
BENCHMARK(BM_iter_n)->DenseRange(6, 12)->MinTime(2.0);
BENCHMARK(BM_IsMmsFeasible)->DenseRange(6, 12)->MinTime(2.0);

BENCHMARK_MAIN();
