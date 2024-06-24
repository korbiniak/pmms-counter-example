#include <alloca.h>
#include <atomic>
#include <cstdlib>
#include <iomanip>
#include <ios>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

#include "src/cxxopts.h"
#include "src/generator.h"
#include "src/pmms.h"

struct Args {
  int n;
  int m;
  int thread_nr;
  valuation_t min_val;
  valuation_t max_val;
  Generator generator;
  bool monotone;
};

std::size_t get_max_number_width(valuation_t max_val) {
  std::cout << std::fixed << std::setprecision(3);
  std::ostringstream strs;
  strs << std::fixed << std::setprecision(3) << max_val;
  return strs.str().size();
}

void pinToCpu(int cpu) {
  cpu_set_t cpuset;
  CPU_ZERO(&cpuset);
  CPU_SET(cpu, &cpuset);

  pthread_t thread = pthread_self();
  if (pthread_setaffinity_np(thread, sizeof(cpu_set_t), &cpuset) != 0) {
    std::cerr << "Error setting thread affinity\n";
  }

  // // Check CPU again after setting affinity
  cpu = sched_getcpu();
  std::cout << "Thread pinned to CPU: " << cpu << std::endl;
}

/* Run an infinite loop.
   * 1. Generate a random valuation
   * 2. Check the number of allocations
   * 2b. If it's lower than the previous one then set the new lowest
   * number of allocations.
   */
void randomValuationAllocationsCount(Args args, std::atomic<int>& min_count,
                                     std::mutex& log_mtx) {
  int n = args.n;
  int m = args.m;
  int thread_nr = args.thread_nr;
  valuation_t min_val = args.min_val;
  valuation_t max_val = args.max_val;
  Generator& generator = args.generator;
  bool monotone = args.monotone;
  const std::size_t number_width = get_max_number_width(max_val);

  // pinToCpu(thread_nr);
  std::cout << "Starting on thread " << thread_nr << std::endl;

  for (int cnt = 0;; cnt++) {
    if (cnt % 5000000 == 0) {
      std::cout << cnt << " valuations checked on thread " << thread_nr
                << std::endl;
    }
    int current_min = min_count.load();

    std::vector<Valuation> valuations;
    if (monotone) {
      valuations = generator.monotoneValuations(n, m, min_val, max_val);
    } else {
      valuations = generator.additiveValuations(n, m, min_val, max_val);
    }

    std::vector<Allocation> allocations =
        Pmms::getAllAllocationsPrecomputeMu(valuations, current_min);

    int current = (int)allocations.size();

    while (current < current_min) {
      if (min_count.compare_exchange_weak(current_min, current)) {
        // current_min is automatically updated if compare_exchange_weak fails
        std::lock_guard<std::mutex> guard(log_mtx);

        std::cout << "Found new minimum on thread " << thread_nr << std::endl;

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

  std::cout << "Finishing on thread " << thread_nr << std::endl;
}

void parseCommandLineAndRun(int argc, char* argv[]) {
  /* Parse command line arguments. */
  cxxopts::Options options(argv[0], "PMMS examples generator.");
  options.add_options()("h,help", "Print help")("n,agents", "Number of agents",
                                                cxxopts::value<int>())(
      "m,items", "Number of items", cxxopts::value<int>())(
      "min", "Minimum valuation value",
      cxxopts::value<valuation_t>()->default_value("1"))(
      "max", "Maximum valuation value",
      cxxopts::value<valuation_t>()->default_value("100"))(
      "p,threads", "Number of threads to run on",
      cxxopts::value<int>()->default_value("1"))("monotone",
                                                 "Use monotone valuations.");

  auto result = options.parse(argc, argv);

  if (result.count("help")) {
    std::cout << options.help() << std::endl;
    exit(EXIT_SUCCESS);
  }

  int n = result["agents"].as<int>();
  int m = result["items"].as<int>();
  int threads_num = result["threads"].as<int>();
  valuation_t min_val = result["min"].as<valuation_t>();
  valuation_t max_val = result["max"].as<valuation_t>();
  bool monotone = result.count("monotone");

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

  std::cout << "Searching random valuations for n=" << n << " m=" << m
            << " on p=" << threads_num << " threads." << std::endl;

  std::atomic<int> min_allocs(std::numeric_limits<int>::max());
  std::mutex log_mtx;  // Mutex for synchronizing additional actions

  std::vector<std::thread> threads;

  auto start = std::chrono::high_resolution_clock::now();

  for (int i = 0; i < threads_num; ++i) {
    Args args = {.n = n,
                 .m = m,
                 .thread_nr = i,
                 .min_val = min_val,
                 .max_val = max_val,
                 .generator = Generator(),
                 .monotone = monotone};
    threads.emplace_back(randomValuationAllocationsCount, args,
                         std::ref(min_allocs), std::ref(log_mtx));
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
    parseCommandLineAndRun(argc, argv);
  } catch (const cxxopts::exceptions::exception& e) {
    std::cerr << "Error parsing option: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
