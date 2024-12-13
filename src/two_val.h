#ifndef TWOVAL_H
#define TWOVAL_H

#include <memory>
#include <vector>

#include "src/config.h"

/* I assume that a valuation cannot be empty, i.e. number of items is greater than 0. */
class TwoVal {
 private:
  std::size_t m;
  std::vector<std::vector<valuation_t>> v;

 public:
  TwoVal(const std::vector<valuation_t>& v_);
  TwoVal(std::size_t m_, const std::vector<std::vector<valuation_t>>& v_);
  TwoVal(std::size_t m_, std::vector<std::vector<valuation_t>>&& v_);
  TwoVal(const TwoVal& other);
  TwoVal(TwoVal&& other) noexcept = default;
  TwoVal& operator=(const TwoVal& other);
  TwoVal& operator=(TwoVal&& other) noexcept = default;
  ~TwoVal() = default;

  valuation_t operator[](const std::pair<int, int>& idx) const;

  bool operator==(const TwoVal& other) const;

  inline std::size_t length() const { return m; }

  /* This makes sense only if the valuation is additive and m >= 2. */
  std::vector<valuation_t> get_v() const;

  void monotoneDump(std::ostream& os, const std::size_t& number_width) const;
  void additiveDump(std::ostream& os, const std::size_t& number_width) const;
};

#endif
