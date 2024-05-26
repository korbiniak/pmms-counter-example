#include "src/allocation.h"

#include <cassert>
#include <vector>
#include "bundle.h"

Allocation::Allocation(const std::vector<bundle_t>& bundles_,
                       const std::size_t& m_)
    : bundles(bundles_), m(m_) {
  debugCheckIntegrity();
}

Allocation::Allocation(std::vector<bundle_t>&& bundles_, const std::size_t& m_)
    : bundles(std::move(bundles_)), m(m_) {
  debugCheckIntegrity();
}

bundle_t& Allocation::operator[](const std::size_t& idx) {
  return bundles[idx];
}

const bundle_t& Allocation::operator[](const std::size_t& idx) const {
  return bundles[idx];
}

void Allocation::iter3(const size_t& m,
                       const std::function<void(const Allocation&)>& func) {
  bundle_t all_items = (1 << m) - 1;
  BUNDLE_LOOP(first, all_items, {
    BUNDLE_LOOP(second, (~first) & all_items, {
      bundle_t third = ~first & (~second) & all_items;
      func(Allocation({first, second, third}, m));
    });
  });
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

void Allocation::dump(std::ostream& os) const {
  for (std::size_t i = 0; i < agents(); i++) {
    os << i << ": {";
    for (auto j : Bundle::toItemsList(bundles[i], m)) {
      os << j << ", ";
    }
    os << "}\n";
  }
}
