#include <cassert>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "allocation.h"
#include "src/cxxopts.h"
#include "src/efx.h"
#include "src/generator.h"
#include "src/pmms.h"
#include "src/two_alloc.h"

// Helper function to parse a string representation of a set into an actual set
bundle_t parseSet(const std::string& set_str) {
  bundle_t result = 0;
  std::stringstream ss(set_str.substr(1, set_str.length() - 2));  // Remove { }
  std::string item;

  while (std::getline(ss, item, ',')) {
    if (!item.empty()) {
      result = Bundle::addItem(result, std::stoi(item) - 1);
    }
  }

  return result;
}

// Main processing function
std::vector<Valuation> process_input(const std::string& input_str, size_t n,
                                     size_t m) {
  std::vector<Valuation> result;
  std::stringstream ss(input_str);
  std::string line;
  std::vector<std::string> valuations;
  std::string current_valuation;

  // Split input into valuations (separated by blank lines)
  while (std::getline(ss, line)) {
    if (line.empty()) {
      if (!current_valuation.empty()) {
        valuations.push_back(current_valuation);
        current_valuation.clear();
      }
    } else {
      if (!current_valuation.empty()) {
        current_valuation += "\n";
      }
      current_valuation += line;
    }
  }
  if (!current_valuation.empty()) {
    valuations.push_back(current_valuation);
  }

  assert(n == valuations.size());

  // Process each valuation
  for (size_t i = 0; i < n; i++) {
    std::unique_ptr<valuation_t[]> val =
        std::unique_ptr<valuation_t[]>(new valuation_t[1 << m]);

    std::stringstream valuation_ss(valuations[i]);
    std::string valuation_line;
    bool first_line = true;

    std::string valuation_key = "Valuation " + std::to_string(i);

    // Process each line in the valuation
    while (std::getline(valuation_ss, valuation_line)) {
      if (first_line) {  // Skip the "Valuation X:" line
        first_line = false;
        continue;
      }

      // Find the colon separator
      size_t colon_pos = valuation_line.find(": ");
      if (colon_pos == std::string::npos)
        continue;

      // Split line into key and value
      std::string key = valuation_line.substr(0, colon_pos);
      std::string value_str = valuation_line.substr(colon_pos + 2);

      // Parse the set and value
      bundle_t key_set = parseSet(key);
      int value = std::stoi(value_str);

      val[key_set] = value;
    }

    result.push_back(Valuation(m, std::move(val)));
  }

  return result;
}

void parseCommandLineAndRun(int argc, char* argv[]) {
  (void)argc;
  (void)argv;

  int n = 3, m = 6;
  std::string input;
  std::string line;
  while (std::getline(std::cin, line)) {
    input += line + "\n";
  }

  std::vector<Valuation> valuations = process_input(input, n, m);
  std::vector<Allocation> allocations = Pmms::getAllAllocations(valuations);

  std::cout << "Found " << allocations.size() << " allocations.\n";
  for (const Allocation& allocation : allocations) {
    allocation.dump(std::cout);
    std::cout << std::endl;
  }
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
