#include "src/valuation.h"

#include <stdint.h>
#include <cassert>
#include <memory>
#include <vector>

Valuation::Valuation(const std::vector<valuation_t>& v_) : m(v_.size()) {
  assert(v_.size() > 0);
  std::size_t sz = (1 << v_.size());
  v = std::unique_ptr<valuation_t[]>(new valuation_t[sz]);
  for (int mask = 0; mask < (int)sz; mask++) {
    v[mask] = 0;
    for (int i = 0; i < (int)m; i++) {
      if ((1 << i) & mask) {
        v[mask] += v_[i];
      }
    }
  }
}

Valuation::Valuation(const Valuation& other) : m(other.m) {
  v = std::unique_ptr<valuation_t[]>(new valuation_t[(1 << m)]);
  for (int mask = 0; mask < (1 << m); mask++) {
    v[mask] = other.v[mask];
  }
}

Valuation& Valuation::operator=(const Valuation& other) {
  if (this == &other) {
    return *this;
  }

  m = other.m;
  v = std::unique_ptr<valuation_t[]>(new valuation_t[(1 << m)]);
  for (int mask = 0; mask < (1 << m); mask++) {
    v[mask] = other.v[mask];
  }

  return *this;
}

valuation_t Valuation::operator[](const bundle_t& bundle) const {
  return v[bundle];
}

bool Valuation::operator==(const Valuation& other) const {
  if (m != other.m) {
    return false;
  }
  for (std::size_t i = 0; i < m; i++) {
    if (v[1 << i] != other.v[1 << i]) {
      return false;
    }
  }
  return true;
}

std::vector<valuation_t> Valuation::get_v() const {
  std::vector<valuation_t> result;

  for (std::size_t i = 0; i < m; i++) {
    result.push_back(v[1 << i]);
  }
  return result;
}

Valuation& Valuation::normalize(const valuation_t& normal_value) {
  valuation_t sum = v[(1 << m) - 1];

  for (int mask = 0; mask < (1 << m); mask++) {
    v[mask] = v[mask] * normal_value / sum;
  }

  return *this;
}
