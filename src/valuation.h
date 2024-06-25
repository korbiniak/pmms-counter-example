#ifndef VALUATION_H
#define VALUATION_H

#include <memory>
#include <vector>

#include "src/bundle.h"
#include "src/config.h"

/* I assume that a valuation cannot be empty, i.e. number of items is greater than 0. */
class Valuation {
 private:
  std::size_t m;
  std::unique_ptr<valuation_t[]> v;

 public:
  Valuation(const std::vector<valuation_t>& v_);
  Valuation(std::size_t m, std::unique_ptr<valuation_t[]> v);
  Valuation(const Valuation& other);
  Valuation(Valuation&& other) noexcept = default;
  Valuation& operator=(const Valuation& other);
  Valuation& operator=(Valuation&& other) noexcept = default;
  ~Valuation() = default;

  valuation_t operator[](const bundle_t& bundle) const;

  bool operator==(const Valuation& other) const;

  inline std::size_t length() const { return m; }

  /* This makes sense only if the valuation is additive. */
  std::vector<valuation_t> get_v() const;

  /* TODO: This won't work well if valuation_t is not floating point. */
  Valuation& normalize(const valuation_t& normal_value = 1);

  void monotoneDump(std::ostream& os) const;
  void additiveDump(std::ostream& os, const std::size_t& number_width) const;
};

#endif
