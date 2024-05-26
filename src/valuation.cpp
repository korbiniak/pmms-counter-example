#include "src/valuation.h"

#include <vector>

Valuation::Valuation(const std::vector<valuation_t>& v) : v(v) {}

valuation_t Valuation::operator[](const bundle_t& bundle) const {
  valuation_t result = 0;
  for (std::size_t i = 0; i < v.size(); i++) {
    if (Bundle::hasItem(bundle, i)) {
      result += v[i];
    }
  }
  return result;
}
