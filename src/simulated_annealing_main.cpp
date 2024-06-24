#include <chrono>
#include <cstdlib>
#include <iomanip>
#include <vector>

#include "src/cxxopts.h"
#include "src/generator.h"
#include "src/pmms.h"
#include "src/simulated_annealing.h"

std::size_t get_max_number_width(valuation_t max_val) {
  std::cout << std::fixed << std::setprecision(3);
  std::ostringstream strs;
  strs << std::fixed << std::setprecision(3) << max_val;
  return strs.str().size();
}

void parseCommandLineAndRun(int argc, char* argv[]) {
  /* Parse command line arguments. */
  cxxopts::Options options(argv[0], "PMMS examples generator.");
  options.add_options()("h,help", "Print help")(
      "n,agents", "Number of agents",
      cxxopts::value<int>()->default_value("3"))("m,items", "Number of items",
                                                 cxxopts::value<int>())(
      "i,iters", "Number of iterations", cxxopts::value<std::size_t>())(
      "a,alpha", "Alpha parameter", cxxopts::value<double>())(
      "u,mean", "Mean parameter", cxxopts::value<double>()->default_value("1"))(
      "s,stddev", "Stddev parameter",
      cxxopts::value<double>()->default_value("1"))(
      "min", "Minimum valuation value",
      cxxopts::value<valuation_t>()->default_value("1"))(
      "max", "Maximum valuation value",
      cxxopts::value<valuation_t>()->default_value("100"));

  auto result = options.parse(argc, argv);

  if (result.count("help")) {
    std::cout << options.help() << std::endl;
    exit(EXIT_SUCCESS);
  }

  int n = result["agents"].as<int>();
  int m = result["items"].as<int>();
  std::size_t max_iterations = result["iters"].as<std::size_t>();
  double alpha = result["alpha"].as<double>();
  double mean = result["mean"].as<double>();
  double stddev = result["stddev"].as<double>();
  valuation_t min_val = result["min"].as<valuation_t>();
  valuation_t max_val = result["max"].as<valuation_t>();

  if (n != 3) {
    std::cerr << "Only 3 agents are supported now." << std::endl;
    exit(EXIT_FAILURE);
  }
  if (m > 20) {
    std::cerr << "WARNING: valuations are computed for every subset of "
                 "items, hence it takes a lot of space to hold a valuation for "
                 "more than 20 items. Please proceed with caution."
              << std::endl;
  }

  std::cout << "Simulated annealing for n=" << n << " m=" << m << " a=" << alpha
            << std::endl;

  auto start = std::chrono::high_resolution_clock::now();

  Generator gen;
  std::vector<Valuation> valuations =
      gen.additiveValuations(n, m, min_val, max_val, /*normalize=*/100);

  valuations = SimulatedAnnealing::simulatedAnnealing(
      valuations, gen, max_iterations, alpha, mean, stddev, 100,
      [](const std::vector<Valuation>& vals) {
        return -Pmms::minimalEnvyOverAllAllocations(vals);
      });

  auto stop = std::chrono::high_resolution_clock::now();

  for (int i = 0; i < n; i++) {
    std::cout << i << ": ";
    for (auto& v : valuations[i].get_v()) {
      std::cout << v << " ";
    }
    std::cout << std::endl;
  }
  std::cout << "Number of allocations: "
            << Pmms::getAllAllocationsPrecomputeMu(valuations).size()
            << std::endl;
  std::cout << "Min envy: " << Pmms::minimalEnvyOverAllAllocations(valuations)
            << std::endl;

  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
  std::cout << "Time taken: " << duration.count() << " milliseconds"
            << std::endl;
}

int main(int argc, char* argv[]) {
  try {
    parseCommandLineAndRun(argc, argv);
  } catch (const cxxopts::exceptions::exception& e) {
    std::cerr << "Error parsing option: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
