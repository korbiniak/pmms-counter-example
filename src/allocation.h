#ifndef ALLOCATION_H
#define ALLOCATION_H

#include <vector>

#include "src/config.h"

class Allocation {
 private:
  std::vector<bundle_t> bundles;
  int m;

 public:
  Allocation(const std::vector<bundle_t>& bundles, const int& m);
  Allocation(std::vector<bundle_t>&& bundle, const int& m);
  Allocation(const Allocation& other) = default;
  Allocation(Allocation&& other) noexcept = default;
  Allocation& operator=(const Allocation& other) = default;
  Allocation& operator=(Allocation&& other) noexcept = default;

  void debugCheckIntegrity();
};

#endif
