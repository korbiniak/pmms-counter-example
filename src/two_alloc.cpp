#include "src/two_alloc.h"

#include <algorithm>
#include <cassert>

TwoAlloc::TwoAlloc(const two_alloc_t& bundles_) : bundles(bundles_) {}

TwoAlloc::TwoAlloc(two_alloc_t&& bundles_) : bundles(std::move(bundles_)) {}

std::pair<int, int>& TwoAlloc::operator[](const std::size_t& idx) {
  return bundles[idx];
}

const std::pair<int, int>& TwoAlloc::operator[](const std::size_t& idx) const {
  return bundles[idx];
}

bool TwoAlloc::iter_n_aux(int n, std::vector<bool>& avail_items,
                          TwoAlloc& allocation,
                          const std::function<bool(const TwoAlloc&)>& func) {
  if (n == 0) {
    return func(allocation);
  }

  for (uint i = 0; i < avail_items.size(); i++) {
    if (!avail_items[i]) {
      continue;
    }
    avail_items[i] = 0;
    for (uint j = i + 1; j < avail_items.size(); j++) {
      if (!avail_items[j]) {
        continue;
      }

      allocation.bundles.push_back({i, j});
      avail_items[j] = 0;
      if (!iter_n_aux(n - 1, avail_items, allocation, func)) {
        return false;
      }
      avail_items[j] = 1;
      allocation.bundles.pop_back();
    }
    avail_items[i] = 1;
  }

  return true;
}

void TwoAlloc::iter_n(const int& n,
                      const std::function<bool(const TwoAlloc&)>& func) {
  std::vector<bool> avail_items(n * 2, true);
  TwoAlloc two_alloc({});

  iter_n_aux(n, avail_items, two_alloc, func);
}

void TwoAlloc::dump(std::ostream& os) const {
  for (size_t i = 0; i < bundles.size(); ++i) {
    os << "Agent " << i << ": ";
    os << bundles[i].first << ", " << bundles[i].second;
    os << "\n";
  }
}
