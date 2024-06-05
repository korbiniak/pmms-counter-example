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

BENCHMARK(BM_PmmsGetAllAllocations)->DenseRange(6, 12);

BENCHMARK_MAIN();
