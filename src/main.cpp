#include <alloca.h>
#include <cstdlib>
#include <iomanip>
#include <ios>
#include <vector>

#include "src/cxxopts.h"
#include "src/generator.h"
#include "src/pmms.h"

void parseCommandLineAndRun(int argc, char* argv[]) {
  /* Parse command line arguments. */
  cxxopts::Options options(argv[0], "PMMS examples generator.");
  options.add_options()("h,help", "Print help")(
      "s,seed", "RNG seed", cxxopts::value<int>()->default_value("-1"))(
      "n,agents", "Number of agents", cxxopts::value<int>())(
      "m,items", "Number of items", cxxopts::value<int>())(
      "min", "Minimum valuation value",
      cxxopts::value<valuation_t>()->default_value("1"))(
      "max", "Maximum valuation value",
      cxxopts::value<valuation_t>()->default_value("100"));

  auto result = options.parse(argc, argv);

  if (result.count("help")) {
    std::cout << options.help() << std::endl;
    exit(EXIT_SUCCESS);
  }

  int seed = result["seed"].as<int>();
  int n = result["agents"].as<int>();
  int m = result["items"].as<int>();
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

  Generator generator(seed);

  /* Prepare printing settings. */
  std::cout << std::fixed << std::setprecision(3);
  std::ostringstream strs;
  strs << std::fixed << std::setprecision(3) << max_val;
  const std::size_t number_width = strs.str().size();

  std::size_t min_allocs = 1e9;
  std::size_t nr_of_min_allocs = 0;

  std::cout << "Searching random valuations for n=" << n << " m=" << m
            << " and with seed=" << generator.seed << std::endl;

  /* Run an infinite loop.
   * 1. Generate a random valuation
   * 2. Check the number of allocations
   * 2a. If it's the same as the previous lowest one then just adjust the counter.
   * 2b. otherwise set the new lowest number of allocations and reset the counter.
   */
  for (int cnt = 0;; cnt++) {
    if (cnt % 10000 == 0) {
      std::cout << "Allocation nr " << cnt
                << ", min allocs found: " << nr_of_min_allocs << std::endl;
    }
    std::vector<Valuation> valuations =
        generator.generateRandomValuations(n, m, min_val, max_val);

    std::vector<Allocation> allocations =
        Pmms::getAllAllocationsPrecomputeMu(valuations);

    if (allocations.size() == min_allocs) {
      nr_of_min_allocs++;
    } else if (allocations.size() < min_allocs) {
      min_allocs = allocations.size();
      nr_of_min_allocs = 1;

      for (int i = 0; i < n; i++) {
        std::cout << i << ": ";
        for (auto& v : valuations[i].get_v()) {
          std::cout << std::setw(number_width + 1) << v;
        }
        std::cout << std::endl;
      }
      std::cout << "Number of allocations: "
                << Pmms::getAllAllocationsPrecomputeMu(valuations).size()
                << std::endl;
    }
  }
}

int main(int argc, char* argv[]) {
  try {
    parseCommandLineAndRun(argc, argv);
  } catch (const cxxopts::exceptions::exception& e) {
    std::cerr << "Error parsing option: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  std::cout << "Hello, world!\n";
  return EXIT_SUCCESS;
}
