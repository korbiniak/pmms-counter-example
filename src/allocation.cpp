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

void Allocation::iter_3(const size_t& m,
                        const std::function<bool(const Allocation&)>& func) {
  bundle_t all_items = (1 << m) - 1;
  Allocation allocation({}, m);

  BUNDLE_LOOP(first, all_items, {
    allocation.bundles.push_back(first);
    BUNDLE_LOOP(second, (~first) & all_items, {
      bundle_t third = ~first & (~second) & all_items;
      allocation.bundles.push_back(second);
      allocation.bundles.push_back(third);
      if (!func(allocation)) {
        return;
      }
      allocation.bundles.pop_back();
      allocation.bundles.pop_back();
    });
    allocation.bundles.pop_back();
  });
}

bool Allocation::iter_n_aux(
    int n, bundle_t avail_items, Allocation& allocation,
    const std::function<bool(const Allocation&)>& func) {
  /* If we're the only agent left, give us all the items, run [func] and terminate. */
  if (n == 1) {
    allocation.bundles.push_back(avail_items);
    if (!func(allocation)) {
      return false;
    }
    allocation.bundles.pop_back();
    return true;
  }

  BUNDLE_LOOP(bundle, avail_items, {
    allocation.bundles.push_back(bundle);
    if (!iter_n_aux(n - 1, avail_items & (~bundle), allocation, func)) {
      return false;
    }
    allocation.bundles.pop_back();
  });

  return true;
}

void Allocation::iter_n(const int& n, const size_t& m,
                        const std::function<bool(const Allocation&)>& func) {
  if (n == 0) {
    return;
  }

  if (n == 3) {
    iter_3(m, func);
    return;
  }

  Allocation allocation({}, m);
  (void)iter_n_aux(n, (1 << m) - 1, allocation, func);
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
    for (int j : Bundle::toItemsList(bundles[i], m)) {
      os << j << ", ";
    }
    os << "}" << std::endl;
  }
}
