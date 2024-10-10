#include "src/efx.h"

#include "src/bundle.h"

namespace Efx {

bool isEnvious(const bundle_t& b1, const bundle_t& b2,
               const Valuation& valuation) {
  valuation_t my_value = valuation[b1];
  for (uint j = 0; j < valuation.length(); j++) {
    if (Bundle::hasItem(b2, j)) {
      if (valuation[Bundle::removeItem(b2, j)] > my_value) {
        return true;
      }
    }
  }
  return false;
}

bool isEnvyFree(const Allocation& allocation,
                const std::vector<Valuation>& valuations) {
  for (uint i = 0; i < valuations.size(); i++) {
    for (uint j = 0; j < valuations.size(); j++) {
      if (i == j) {
        continue;
      }
      if (isEnvious(allocation[i], allocation[j], valuations[i])) {
        return false;
      }
    }
  }

  return true;
}

/* TODO: abstract this for pmms and efx, this is basically the same code. */
std::vector<Allocation> getAllAllocations(
    const std::vector<Valuation>& valuations, const std::size_t& at_most) {
#ifndef NDEBUG
  for (uint i = 0; i < valuations.size(); i++) {
    assert(valuations[i].length() == valuations[0].length());
  }
#endif

  std::vector<Allocation> result;
  int n = valuations.size();
  int m = valuations[0].length();
  std::size_t found = 0;
  Allocation::iter_n(n, m, [&](const Allocation& allocation) {
    if (isEnvyFree(allocation, valuations)) {
      result.push_back(allocation);
      if (at_most > 0 && ++found >= at_most) {
        return false;
      }
    }
    return true;
  });

  return result;
}

};  // namespace Efx
