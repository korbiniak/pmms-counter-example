#include "src/allocation.h"

#include <cassert>
#include <vector>

Allocation::Allocation(const std::vector<bundle_t>& bundles_, const int& m_)
    : bundles(bundles_), m(m_) {
  debugCheckIntegrity();
}

Allocation::Allocation(std::vector<bundle_t>&& bundles_, const int& m_)
    : bundles(std::move(bundles_)), m(m_) {
  debugCheckIntegrity();
}

void Allocation::debugCheckIntegrity() {
#ifndef NDEBUG
  bundle_t sum = 0;
  for (const auto& bundle : bundles) {
    assert(sum & (bundle == 0));
    sum &= bundle;
  }
  assert(sum == ((1UL << m) - 1));
#endif /* NDEBUG */
}
