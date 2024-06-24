#include "src/generator.h"

#include <algorithm>
#include <random>

#include "src/config.h"

Generator::Generator(int seed_) {
  if (seed_ == -1) {
    seed_ = std::random_device()();
  }
  seed = seed_;
  gen = std::mt19937(seed_);
}

Valuation Generator::additiveValuation(const std::size_t& items,
                                       const valuation_t& min,
                                       const valuation_t& max,
                                       valuation_t normalize) {
#ifdef VALUATION_T_INT64
  std::uniform_int_distribution<> distr(min, max);
#else
  std::uniform_real_distribution<> distr(min, max);
#endif

  std::vector<valuation_t> v(items);
  for (uint j = 0; j < items; ++j) {
    v[j] = distr(gen);
  }
  return Valuation(v).normalize(normalize);
}

std::vector<Valuation> Generator::additiveValuations(const std::size_t& agents,
                                                     const std::size_t& items,
                                                     const valuation_t& min,
                                                     const valuation_t& max,
                                                     valuation_t normalize) {
  std::vector<Valuation> valuations;
  for (uint i = 0; i < agents; i++) {
    valuations.push_back(
        additiveValuation(items, min, max).normalize(normalize));
  }
  return valuations;
}

Valuation Generator::monotoneValuation(const std::size_t& items,
                                       const valuation_t& min_delta,
                                       const valuation_t& max_delta,
                                       valuation_t normalize) {
#ifdef VALUATION_T_INT64
  std::uniform_int_distribution<> distr(min_delta, max_delta);
#else
  std::uniform_real_distribution<> distr(min_delta, max_delta);
#endif

  std::unique_ptr<valuation_t[]> v =
      std::unique_ptr<valuation_t[]>(new valuation_t[1 << items]);

  for (int mask = 0; mask < (1 << items); mask++) {
    valuation_t maximal = 0;
    for (uint j = 0; j < items; j++) {
      if ((1 << j) & mask) {
        maximal = std::max(maximal, v[mask & (~(1 << j))]);
      }
    }
    v[mask] = maximal + distr(gen);
  }

  return Valuation(items, std::move(v)).normalize(normalize);
}

std::vector<Valuation> Generator::monotoneValuations(
    const std::size_t& agents, const std::size_t& items,
    const valuation_t& min_delta, const valuation_t& max_delta,
    valuation_t normalize) {
  std::vector<Valuation> valuations;
  for (uint i = 0; i < agents; i++) {
    valuations.push_back(
        monotoneValuation(items, min_delta, max_delta).normalize(normalize));
  }
  return valuations;
}

std::vector<Valuation> Generator::randomNeighbour(
    const std::vector<Valuation>& valuations, const double& mean,
    const double& stddev, valuation_t normalize) {
  std::normal_distribution<> distr(mean, stddev);
  std::vector<Valuation> result;

  for (uint i = 0; i < valuations.size(); i++) {
    std::vector<valuation_t> v = valuations[i].get_v();
    std::vector<valuation_t> new_v(v.size());
    for (uint j = 0; j < v.size(); j++) {
      new_v[j] = std::max((valuation_t)0, (valuation_t)(v[j] + distr(gen)));
    }
    result.push_back(Valuation(new_v));
    if (normalize > 0) {
      result.back().normalize(normalize);
    }
  }

  return result;
}

double Generator::uniform(double min, double max) {
  std::uniform_real_distribution<> distr(min, max);
  return distr(gen);
}
