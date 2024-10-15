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

  static bool iter_n_aux(int n, bundle_t avail_items, Allocation& allocation,
                         const std::function<bool(const Allocation&)>& func);

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

  /* Iterate over all allocations for 3 agents and m items. This
     terminates early if [func] returns [false] at any point. */
  static void iter_3(const size_t& m,
                     const std::function<bool(const Allocation&)>& func);

  /* Iterate over all allocations for n agents and m items. This
     terminates early if [func] returns [false] at any point. */
  static void iter_n(const int& n, const size_t& m,
                     const std::function<bool(const Allocation&)>& func);

  std::vector<std::vector<valuation_t>> valuationMatrix(
      const std::vector<Valuation>& valuations) const;

  void debugCompatibleValuations(
      const std::vector<Valuation>& valuations) const;

  void dump(std::ostream& os) const;
};

#endif
