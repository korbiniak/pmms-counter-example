#include "src/allocation.h"

#include <vector>

Allocation::Allocation(const std::vector<bundle_t>& bundles, const int& m)
    : bundles(bundles), m(m) {
  debugCheckIntegrity();
}

Allocation::Allocation(std::vector<bundle_t>&& bundles, const int& m)
    : bundles(std::move(bundles)), m(m) {
  debugCheckIntegrity();
}

void Allocation::debugCheckIntegrity() {
#ifdef DEBUG
  bundles_t sum = 0;
  for (const auto& bundle : bundles) {
    assert(sum & bundle == 0);
    sum &= bundle;
  }
  assert(sum == ((1 << m) - 1));
#endif /* DEBUG */
}
