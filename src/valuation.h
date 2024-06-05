#ifndef VALUATION_H
#define VALUATION_H

#include <vector>

#include "src/bundle.h"
#include "src/config.h"

/* I assume that a valuation cannot be empty, i.e. number of items is greater than 0. */
class Valuation {
 private:
  std::size_t m;
  valuation_t* v;

 public:
  Valuation(const std::vector<valuation_t>& v_);
  Valuation(const Valuation& other);
  Valuation(Valuation&& other) noexcept = default;
  Valuation& operator=(const Valuation& other);
  Valuation& operator=(Valuation&& other) noexcept = default;
  ~Valuation();

  valuation_t operator[](const bundle_t& bundle) const;

  inline std::size_t length() const { return m; }

  inline std::vector<valuation_t> get_v() const {
    std::vector<valuation_t> result;
    for (std::size_t i = 0; i < m; i++) {
      result.push_back(v[1 << i]);
    }
    return result;
  }

  /* TODO: This won't work well if valuation_t is not floating point. */
  Valuation& normalize(const valuation_t& normal_value = 1);
};

#endif
