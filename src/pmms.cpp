#include "src/pmms.h"

#include <cassert>
#include <iostream>

#include "allocation.h"
#include "src/config.h"

namespace Pmms {
/* TODO: This could be done much faster if we used ints for valuation +
   the sum of items is low. */
std::pair<bundle_t, bundle_t> mu(const bundle_t& b1, const bundle_t& b2,
                                 const Valuation& valuation) {
  return mu(b1 | b2, valuation);
}

std::pair<bundle_t, bundle_t> mu(const bundle_t& b,
                                 const Valuation& valuation) {
  bundle_t best_bundle = 0;
  valuation_t best_value = 0;

  BUNDLE_LOOP(bundle, b, {
    valuation_t value = valuation[bundle];
    if (value > valuation[b & (~bundle)]) {
      continue;
    }
    if (value > best_value) {
      best_value = value;
      best_bundle = bundle;
    }
  });

  return {best_bundle, b & (~best_bundle)};
}

valuation_t muValue(const bundle_t& b1, const bundle_t& b2,
                    const Valuation& valuation) {
  return valuation[mu(b1 | b2, valuation).first];
}

valuation_t muValue(const bundle_t& b, const Valuation& valuation) {
  return valuation[mu(b, valuation).first];
}

bool isEnvious(const bundle_t& b1, const bundle_t& b2,
               const Valuation& valuation) {
  return valuation[b1] < muValue(b1, b2, valuation);
}

bool isEnvyFree(const Allocation& allocation,
                const std::vector<Valuation>& valuations) {
  int n = allocation.agents();
  std::vector<valuation_t> values(n);

  for (int i = 0; i < n; i++) {
    valuation_t value = valuations[i][allocation[i]];
    for (int j = 0; j < n; j++) {
      if (i == j) {
        continue;
      }
      if (value < muValue(allocation[i], allocation[j], valuations[i])) {
        return false;
      }
    }
  }

  return true;
}

std::vector<Allocation> getAllAllocations(
    const std::vector<Valuation>& valuations, const std::size_t& at_most) {
  std::vector<Allocation> result;
#ifndef NDEBUG
  for (uint i = 0; i < valuations.size(); i++) {
    assert(valuations[i].length() == valuations[0].length());
  }
#endif

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

std::vector<Allocation> getAllAllocationsPrecomputeMu(
    const std::vector<Valuation>& valuations, const std::size_t& at_most) {
  std::vector<Allocation> result;
#ifndef NDEBUG
  for (uint i = 0; i < valuations.size(); i++) {
    assert(valuations[i].length() == valuations[0].length());
  }
#endif

  valuation_t** mu = precomputeMu(valuations);
  int n = valuations.size();
  int m = valuations[0].length();
  std::size_t found = 0;
  Allocation::iter_n(n, m, [&](const Allocation& allocation) {
    if (maximalEnvyPrecomputedMu(allocation, valuations, mu) <= 0.) {
      result.push_back(allocation);
      if (at_most > 0 && ++found >= at_most) {
        return false;
      }
    }
    return true;
  });

  free2DArray(mu);
  return result;
}

valuation_t maximalEnvy(const Allocation& allocation,
                        const std::vector<Valuation>& valuations) {
  valuation_t maximal_envy = MIN_VALUATION;
  for (uint i = 0; i < allocation.agents(); i++) {
    valuation_t maximal_mu_value = MIN_VALUATION;
    for (uint j = 0; j < allocation.agents(); j++) {
      if (i == j) {
        continue;
      }

      maximal_mu_value =
          std::max(maximal_mu_value,
                   muValue(allocation[i], allocation[j], valuations[i]));
    }

    maximal_envy =
        std::max(maximal_envy, maximal_mu_value - valuations[i][allocation[i]]);
  }

  return maximal_envy;
}

valuation_t** precomputeMu(const std::vector<Valuation>& valuations) {
  size_t m = valuations[0].length();
  valuation_t** mu = create2DArray<valuation_t>(valuations.size(), 1 << m);

  for (size_t i = 0; i < valuations.size(); i++) {
    for (int mask = 0; mask < (1 << m); mask++) {
      mu[i][mask] = muValue(mask, valuations[i]);
    }
  }

  return mu;
}

valuation_t maximalEnvyPrecomputedMu(const Allocation& allocation,
                                     const std::vector<Valuation>& valuations,
                                     valuation_t** precomputed_mu) {
  valuation_t maximal_envy = MIN_VALUATION;
  for (uint i = 0; i < allocation.agents(); i++) {
    valuation_t maximal_mu_value = MIN_VALUATION;
    for (uint j = 0; j < allocation.agents(); j++) {
      if (i == j) {
        continue;
      }

      maximal_mu_value = std::max(
          maximal_mu_value, precomputed_mu[i][allocation[i] | allocation[j]]);
    }

    maximal_envy =
        std::max(maximal_envy, maximal_mu_value - valuations[i][allocation[i]]);
  }

  return maximal_envy;
}

valuation_t minimalEnvyOverAllAllocations(
    const std::vector<Valuation>& valuations) {
  valuation_t** mu = precomputeMu(valuations);
  valuation_t min_envy = MAX_VALUATION;
  int n = valuations.size();
  int m = valuations[0].length();

  Allocation::iter_n(n, m, [&](const Allocation& allocation) {
    valuation_t envy = maximalEnvyPrecomputedMu(allocation, valuations, mu);
    if (envy < min_envy) {
      min_envy = envy;
    }
    /* Always proceed to the next allocation. */
    return true;
  });
  free2DArray(mu);

  return min_envy;
}

}; /* namespace Pmms */
