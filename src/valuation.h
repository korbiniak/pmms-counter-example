#ifndef VALUATION_H
#define VALUATION_H

#include <vector>

#include "src/bundle.h"
#include "src/config.h"

/* I assume that a valuation cannot be empty, i.e. number of items is greater than 0. */
class Valuation {
 private:
  std::vector<valuation_t> v;

 public:
  Valuation(const std::vector<valuation_t>& v_);
  Valuation(const Valuation& other) = default;
  Valuation(Valuation&& other) noexcept = default;
  Valuation& operator=(const Valuation& other) = default;
  Valuation& operator=(Valuation&& other) noexcept = default;

  valuation_t operator[](const bundle_t& bundle) const;

  inline std::size_t items() const { return v.size(); }
};
#endif
