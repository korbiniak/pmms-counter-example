#ifndef GENERATOR_H
#define GENERATOR_H

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

  std::vector<Valuation> generateRandomValuations(const std::size_t& agents,
                                                  const std::size_t& items,
                                                  const valuation_t& min,
                                                  const valuation_t& max);
};

#endif /* GENERATOR_H */
