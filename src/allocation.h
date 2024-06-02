#ifndef ALLOCATION_H
#define ALLOCATION_H

#include <functional>
#include <iostream>
#include <vector>

#include "src/config.h"
#include "src/valuation.h"

class Allocation {
 private:
  std::vector<bundle_t> bundles;
  std::size_t m;

 public:
  Allocation(const std::vector<bundle_t>& bundles_, const std::size_t& m_);
  Allocation(std::vector<bundle_t>&& bundles_, const std::size_t& m_);
  Allocation(const Allocation& other) = default;
  Allocation(Allocation&& other) noexcept = default;
  Allocation& operator=(const Allocation& other) = default;
  Allocation& operator=(Allocation&& other) noexcept = default;

  void debugCheckIntegrity();
  inline std::size_t agents() const { return bundles.size(); }

  bundle_t& operator[](const std::size_t& idx);
  const bundle_t& operator[](const std::size_t& idx) const;

  static void iter3(const size_t& m,
                    const std::function<void(const Allocation&)>& func);

  valuation_t maximalEnvy(const std::vector<Valuation>& valuations);

  std::vector<std::vector<valuation_t>> valuationMatrix(
      const std::vector<Valuation>& valuations);

  void debugCompatibleValuations(const std::vector<Valuation>& valuations);

  void dump(std::ostream& os) const;
};

#endif
