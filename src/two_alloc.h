#ifndef TWO_ALLOCATION_H
#define TWO_ALLOCATION_H

#include <functional>
#include <iostream>
#include <utility>
#include <vector>

#include "src/config.h"
#include "src/two_val.h"

class TwoAlloc {
 private:
  using two_alloc_t = std::vector<std::pair<int, int>>;
  two_alloc_t bundles;
  std::size_t m;

  static bool iter_n_aux(int n, std::vector<bool>& avail_items,
                         TwoAlloc& allocation,
                         const std::function<bool(const TwoAlloc&)>& func);

 public:
  TwoAlloc(const two_alloc_t& bundles_);
  TwoAlloc(two_alloc_t&& bundles_);
  TwoAlloc(const TwoAlloc& other) = default;
  TwoAlloc(TwoAlloc&& other) noexcept = default;
  TwoAlloc& operator=(const TwoAlloc& other) = default;
  TwoAlloc& operator=(TwoAlloc&& other) noexcept = default;

  inline std::size_t agents() const { return bundles.size(); }

  std::pair<int, int>& operator[](const std::size_t& idx);
  const std::pair<int, int>& operator[](const std::size_t& idx) const;

  /* Iterate over all allocations for n agents and 2 * n items. This
     terminates early if [func] returns [false] at any point. */
  static void iter_n(const int& n,
                     const std::function<bool(const TwoAlloc&)>& func);

  void dump(std::ostream& os) const;
};

#endif
