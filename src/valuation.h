#ifndef VALUATION_H
#define VALUATION_H

#include <vector>

#include "src/bundle.h"
#include "src/config.h"

class Valuation {
 private:
  std::vector<valuation_t> v;

 public:
  Valuation(const std::vector<valuation_t>& v);
  Valuation() = default;
  Valuation(const Valuation& other) = default;
  Valuation(Valuation&& other) noexcept = default;
  Valuation& operator=(const Valuation& other) = default;
  Valuation& operator=(Valuation&& other) noexcept = default;

  valuation_t operator[](const bundle_t& bundle) const;
};
#endif
