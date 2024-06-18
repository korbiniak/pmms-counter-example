#include "src/simulated_annealing.h"

#include <cmath>

#include "src/pmms.h"

namespace SimulatedAnnealing {

inline double anneal_parameter(double new_cost, double current_cost,
                               double alpha, std::size_t iteration,
                               std::size_t iteration_max) {
  return std::exp(-alpha * (new_cost - current_cost) *
                  ((double)iteration / iteration_max));
}

std::vector<Valuation> simulatedAnnealing(
    const std::vector<Valuation>& initial_valuations, Generator& gen,
    std::size_t max_iterations, double alpha, double mean, double variance,
    valuation_t normalize_value,
    std::function<double(const std::vector<Valuation>&)> cost_function) {
  std::vector<Valuation> current_valuations = initial_valuations;
  double current_cost = cost_function(initial_valuations);

  for (uint i = 0; i < max_iterations; i++) {
    std::vector<Valuation> new_valuations = gen.randomNeighbour(
        current_valuations, mean, variance, normalize_value);

    double new_cost = cost_function(new_valuations);

    if (new_cost < current_cost ||
        anneal_parameter(new_cost, current_cost, alpha, i, max_iterations) >
            gen.uniform(0, 1)) {
      std::cout << "Iteration: " << i << std::endl;
      std::cout << "Jump parameter: "
                << std::exp(-alpha * (new_cost - current_cost) *
                            ((double)i / max_iterations))
                << std::endl;

      current_cost = new_cost;
      current_valuations = new_valuations;

      std::cout
          << "Number of allocations: "
          << Pmms::getAllAllocationsPrecomputeMu(current_valuations).size()
          << std::endl;
      std::cout << "Min envy: "
                << Pmms::minimalEnvyOverAllAllocations(current_valuations)
                << std::endl;
    }
  }

  return current_valuations;
}

}; /* namespace SimulatedAnnealing */
