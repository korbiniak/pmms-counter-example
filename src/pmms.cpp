#include "src/pmms.h"

#include <iostream>

#include "src/config.h"

/* TODO: This could be done much faster if we used ints for valuation +
   the sum of items is low. */
std::pair<bundle_t, bundle_t> Pmms::mu(const bundle_t& b1, const bundle_t& b2,
                                       const Valuation& valuation) {
  bundle_t all_items = b1 | b2;
  bundle_t best_bundle = 0;
  valuation_t entire_value = valuation[all_items];
  valuation_t best_value = 0;

  BUNDLE_LOOP(bundle, all_items, {
    valuation_t value = valuation[bundle];
    if (2 * value > entire_value) {
      continue;
    }
    if (value > best_value) {
      best_value = value;
      best_bundle = bundle;
    }
  });

  return {best_bundle, all_items & (~best_bundle)};
}

valuation_t Pmms::muValue(const bundle_t& b1, const bundle_t& b2,
                          const Valuation& valuation) {
  return valuation[mu(b1, b2, valuation).first];
}

bool Pmms::isEnvious(const bundle_t& b1, const bundle_t& b2,
                     const Valuation& valuation) {
  return valuation[b1] < muValue(b1, b2, valuation);
}

bool Pmms::isEnvyFree(const Allocation& allocation,
                      const std::vector<Valuation>& valuations) {
  int n = allocation.agents();
  std::vector<valuation_t> values(n);
  for (int i = 0; i < n; i++) {
    values[i] = valuations[i][allocation[i]];
  }

  for (int i = 0; i < n; i++) {
    for (int j = i + 1; j < n; j++) {
      auto [left, right] = mu(allocation[i], allocation[j], valuations[i]);
      if (values[i] < valuations[i][left] || values[j] < valuations[j][left]) {
        return false;
      }
    }
  }

  return true;
}

std::vector<Allocation> Pmms::getAllAllocations(
    const std::vector<Valuation>& valuations) {
  std::vector<Allocation> result;
#ifndef NDEBUG
  assert(valuations.size() > 0);
  for (int i = 0; i < valuations.size(); i++) {
    assert(valuations[i].items() == valuations[0].items());
  }
#endif

  int m = valuations[0].length();
  Allocation::iter3(m, [&](const Allocation& allocation) {
    if (isEnvyFree(allocation, valuations)) {
      result.push_back(allocation);
    }
  });

  return result;
}

valuation_t Pmms::maximalEnvy(const Allocation& allocation,
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
