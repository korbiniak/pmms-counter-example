#ifndef SIMULATED_ANNEALING_H
#define SIMULATED_ANNEALING_H

#include <functional>
#include <vector>

#include "src/generator.h"
#include "src/valuation.h"

namespace SimulatedAnnealing {

std::vector<Valuation> simulatedAnnealing(
    const std::vector<Valuation>& initial_valuations, Generator& gen,
    std::size_t max_iterations, double alpha, double mean, double variance,
    valuation_t normalize_value,
    std::function<double(const std::vector<Valuation>&)> cost_function);
};

#endif /* SIMULATED_ANNEALING_H */
