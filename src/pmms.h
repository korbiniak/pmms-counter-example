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
  static std::pair<bundle_t, bundle_t> mu(const bundle_t& b,
                                          const Valuation& valuation);

  static std::pair<bundle_t, bundle_t> mu(const bundle_t& b1,
                                          const bundle_t& b2,
                                          const Valuation& valuation);

  static valuation_t muValue(const bundle_t& b1, const Valuation& valuation);

  static valuation_t muValue(const bundle_t& b1, const bundle_t& b2,
                             const Valuation& valuation);

  static bool isEnvious(const bundle_t& b1, const bundle_t& b2,
                        const Valuation& valuation);

  static bool isEnvyFree(const Allocation& allocation,
                         const std::vector<Valuation>& valuations);

  static std::vector<Allocation> getAllAllocations(
      const std::vector<Valuation>& valuations);

  static std::vector<Allocation> getAllAllocationsPrecomputeMu(
      const std::vector<Valuation>& valuations);

  /* For given allocation find the maximal value over all i, j of
     \mu_i(X_i + X_j, 2) - v_i(X_i).  */
  static valuation_t maximalEnvy(const Allocation& allocation,
                                 const std::vector<Valuation>& valuations);

  /* For given allocation find the maximal value over all i, j of
     \mu_i(X_i + X_j, 2) - v_i(X_i).  */
  static valuation_t maximalEnvyPrecomputedMu(
      const Allocation& allocation, const std::vector<Valuation>& valuations,
      valuation_t** precomputed_mu);

  static valuation_t** precomputeMu(const std::vector<Valuation>& valuations);
};

#endif /* PMMS_H */
