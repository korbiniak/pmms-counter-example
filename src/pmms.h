#ifndef PMMS_H
#define PMMS_H

#include <utility>

#include "src/bundle.h"
#include "src/config.h"
#include "src/valuation.h"

#include "src/allocation.h"

class Pmms {
 private:
 public:
  static std::pair<bundle_t, bundle_t> mu(const bundle_t& b1,
                                          const bundle_t& b2,
                                          const Valuation& valuation);

  static valuation_t muValue(const bundle_t& b1, const bundle_t& b2,
                             const Valuation& valuation);

  static bool isEnvious(const bundle_t& b1, const bundle_t& b2,
                        const Valuation& valuation);

  static bool isEnvyFree(const Allocation& allocation,
                         const std::vector<Valuation>& valuations);
};

#endif /* PMMS_H */
