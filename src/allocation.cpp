#include "src/allocation.h"

#include <cassert>
#include <vector>
#include "bundle.h"
#include "config.h"

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
                       const std::function<bool(const Allocation&)>& func) {
  bundle_t all_items = (1 << m) - 1;
  BUNDLE_LOOP(first, all_items, {
    BUNDLE_LOOP(second, (~first) & all_items, {
      bundle_t third = ~first & (~second) & all_items;
      if (!func(Allocation({first, second, third}, m))) {
        return;
      }
    });
  });
}

void Allocation::debugCheckIntegrity() {
#ifndef NDEBUG
  bundle_t sum = 0;
  for (const auto& bundle : bundles) {
    assert((sum & bundle) == 0);
    sum |= bundle;
  }
  assert(sum == ((1UL << m) - 1));
#endif /* NDEBUG */
}

void Allocation::debugCompatibleValuations(
    __attribute__((unused)) const std::vector<Valuation>& valuations) const {
  assert(valuations.size() == agents());
  assert(valuations[0].length() == m);
}

std::vector<std::vector<valuation_t>> Allocation::valuationMatrix(
    const std::vector<Valuation>& valuations) const {
  debugCompatibleValuations(valuations);

  std::vector<std::vector<valuation_t>> matrix(
      agents(), std::vector<valuation_t>(agents()));

  for (uint i = 0; i < agents(); i++) {
    for (uint j = 0; j < agents(); j++) {
      matrix[i][j] = valuations[i][bundles[j]];
    }
  }

  return matrix;
}

void Allocation::dump(std::ostream& os) const {
  for (std::size_t i = 0; i < agents(); i++) {
    os << i << ": {";
    for (auto j : Bundle::toItemsList(bundles[i], m)) {
      os << j << ", ";
    }
    os << "}" << std::endl;
  }
}
