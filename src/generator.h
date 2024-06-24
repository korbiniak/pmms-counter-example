#ifndef GENERATOR_H
#define GENERATOR_H

#include <functional>
#include <random>

#include <vector>

#include "src/config.h"
#include "src/valuation.h"

class Generator {
 private:
  std::mt19937 gen;

 public:
  int seed; /* Just for debug purposes. */
  Generator(int seed = -1);

  Valuation additiveValuation(const std::size_t& items, const valuation_t& min,
                              const valuation_t& max,
                              valuation_t normalize = 0);

  std::vector<Valuation> additiveValuations(const std::size_t& agents,
                                            const std::size_t& items,
                                            const valuation_t& min,
                                            const valuation_t& max,
                                            valuation_t normalize = 0);

  Valuation monotoneValuation(const std::size_t& items,
                              const valuation_t& min_delta,
                              const valuation_t& max_delta,
                              valuation_t normalize = 0);

  std::vector<Valuation> monotoneValuations(const std::size_t& agents,
                                            const std::size_t& items,
                                            const valuation_t& min_delta,
                                            const valuation_t& max_delta,
                                            valuation_t normalize = 0);

  std::vector<Valuation> randomNeighbour(
      const std::vector<Valuation>& valuations, const double& mean,
      const double& variance, valuation_t normalize = 0);

  double uniform(double min, double max);
};

#endif /* GENERATOR_H */
