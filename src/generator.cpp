#include "src/generator.h"

#include <random>

#include "src/config.h"

Generator::Generator(int seed_) {
  if (seed_ == -1) {
    seed_ = std::random_device()();
  }
  seed = seed_;
  gen = std::mt19937(seed_);
}

std::vector<Valuation> Generator::generateRandomValuations(
    const std::size_t& agents, const std::size_t& items, const valuation_t& min,
    const valuation_t& max) {
#ifdef VALUATION_T_INT64
  std::uniform_int_distribution<> distr(min, max);
#else
  std::uniform_real_distribution<> distr(min, max);
#endif

  std::vector<Valuation> valuations;
  for (uint i = 0; i < agents; ++i) {
    std::vector<valuation_t> valuation(items);
    for (uint j = 0; j < items; ++j) {
      valuation[j] = distr(gen);
    }
    valuations.push_back(Valuation(valuation));
  }

  return valuations;
}
