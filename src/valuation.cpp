#include "src/valuation.h"

#include <cassert>
#include <vector>

Valuation::Valuation(const std::vector<valuation_t>& v_) : v(v_) {
  assert(v.size() > 0);
}

valuation_t Valuation::operator[](const bundle_t& bundle) const {
  valuation_t result = 0;
  for (std::size_t i = 0; i < v.size(); i++) {
    if (Bundle::hasItem(bundle, i)) {
      result += v[i];
    }
  }
  return result;
}

Valuation& Valuation::normalize(const valuation_t& normal_value) {
  valuation_t sum = 0;
  for (const auto& item : v) {
    sum += item;
  }
  for (auto& item : v) {
    item = (item * normal_value) / sum;
  }

  return *this;
}
