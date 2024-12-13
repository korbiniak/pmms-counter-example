#include "src/two_val.h"

#include <iomanip>
#include <stdexcept>
#include <vector>

#include "config.h"

TwoVal::TwoVal(const std::vector<valuation_t>& v_) : m(v_.size()) {
  if (m % 2 != 0) {
    throw std::invalid_argument("Size m must be even");
  }
  v.resize(m, std::vector<valuation_t>(m));
  for (uint i = 0; i < m; i++) {
    for (uint j = 0; j < m; j++) {
      if (i == j) {
        continue;
      }
      v[i][j] = v_[i] + v_[j];
    }
  }
}

TwoVal::TwoVal(std::size_t m_, const std::vector<std::vector<valuation_t>>& v_)
    : m(m_), v(v_) {
  if (m % 2 != 0) {
    throw std::invalid_argument("Size m must be even");
  }
}

TwoVal::TwoVal(std::size_t m_, std::vector<std::vector<valuation_t>>&& v_)
    : m(m_), v(std::move(v_)) {
  if (m % 2 != 0) {
    throw std::invalid_argument("Size m must be even");
  }
}

TwoVal::TwoVal(const TwoVal& other) : m(other.m), v(other.v) {
  if (m % 2 != 0) {
    throw std::invalid_argument("Size m must be even");
  }
}

TwoVal& TwoVal::operator=(const TwoVal& other) {
  if (this == &other) {
    return *this;
  }

  m = other.m;
  v = other.v;

  return *this;
}

bool TwoVal::operator==(const TwoVal& other) const {
  return m == other.m && v == other.v;
}

valuation_t TwoVal::operator[](const std::pair<int, int>& idx) const {
  return v[idx.first][idx.second];
}

std::vector<valuation_t> TwoVal::get_v() const {
  if (m <= 2) {
    return {};
  }

  std::vector<valuation_t> res(m);
  for (uint i0 = 0; i0 < m; i0++) {
    uint i1 = (i0 + 1) % m;
    uint i2 = (i0 + 2) % m;
    valuation_t a = v[i0][i1];
    valuation_t b = v[i1][i2];
    valuation_t c = v[i2][i0];

    res[i0] = (a - b + c) / 2;
  }

  return res;
}

void TwoVal::monotoneDump(std::ostream& os,
                          const std::size_t& number_width) const {
  for (uint i = 0; i < m; i++) {
    os << std::setw(number_width + 1) << i << ": ";
    for (uint j = 0; j < m; j++) {
      os << std::setw(number_width + 1) << v[i][j];
    }
    os << "\n";
  }
}

void TwoVal::additiveDump(std::ostream& os,
                          const std::size_t& number_width) const {
  for (auto& j : get_v()) {
    os << std::setw(number_width + 1) << j;
  }
}
