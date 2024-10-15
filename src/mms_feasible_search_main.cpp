#include <atomic>
#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#include "src/cxxopts.h"
#include "src/generator.h"
#include "src/pmms.h"

struct Args {
  int m;
  int thread_nr;
  valuation_t min_val;
  valuation_t max_val;
  Generator generator;
};

void searchMmsFeasibleValuation(Args args, std::atomic<bool>& found,
                                std::mutex& log_mtx) {
  int m = args.m;
  int thread_nr = args.thread_nr;
  valuation_t min_val = args.min_val;
  valuation_t max_val = args.max_val;
  Generator& generator = args.generator;

  std::cout << "Starting search on thread " << thread_nr << std::endl;

  while (!found.load()) {
    Valuation valuation = generator.monotoneValuation(m, min_val, max_val);

    if (Pmms::isMmsFeasible(valuation)) {
      std::lock_guard<std::mutex> guard(log_mtx);
      if (!found.exchange(true)) {
        std::cout << "Found MMS feasible valuation on thread " << thread_nr
                  << ":" << std::endl;
        valuation.monotoneDump(std::cout);
        std::cout << std::endl;
      }
      break;
    }
  }
}
void runSearch(int argc, char* argv[]) {
  cxxopts::Options options("mms_feasible_search",
                           "MMS feasible valuation search");
  options.add_options()("h,help", "Print help")("m,items", "Number of items",
                                                cxxopts::value<int>())(
      "min", "Minimum valuation value",
      cxxopts::value<valuation_t>()->default_value("1"))(
      "max", "Maximum valuation value",
      cxxopts::value<valuation_t>()->default_value("100"))(
      "p,threads", "Number of threads to run on",
      cxxopts::value<int>()->default_value("1"));

  auto result = options.parse(argc, argv);

  if (result.count("help")) {
    std::cout << options.help() << std::endl;
    return;
  }

  int m = result["items"].as<int>();
  int threads_num = result["threads"].as<int>();
  valuation_t min_val = result["min"].as<valuation_t>();
  valuation_t max_val = result["max"].as<valuation_t>();

  std::cout << "Searching for MMS feasible valuation with m=" << m
            << " on p=" << threads_num << " threads." << std::endl;

  std::atomic<bool> found(false);
  std::mutex log_mtx;
  std::vector<std::thread> threads;

  auto start = std::chrono::high_resolution_clock::now();

  for (int i = 0; i < threads_num; ++i) {
    Args args = {.m = m,
                 .thread_nr = i,
                 .min_val = min_val,
                 .max_val = max_val,
                 .generator = Generator()};
    threads.emplace_back(searchMmsFeasibleValuation, args, std::ref(found),
                         std::ref(log_mtx));
  }

  for (auto& thread : threads) {
    thread.join();
  }

  auto stop = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
  std::cout << "Time taken: " << duration.count() << " milliseconds"
            << std::endl;
}

int main(int argc, char* argv[]) {
  try {
    runSearch(argc, argv);
  } catch (const cxxopts::exceptions::exception& e) {
    std::cerr << "Error parsing option: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
