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
