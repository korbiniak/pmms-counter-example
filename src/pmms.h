#ifndef PMMS_H
#define PMMS_H

#include <utility>

#include "src/config.h"
#include "src/valuation.h"

#include "src/allocation.h"

namespace Pmms {

/* mu(b, v) is the max over A, B partitions of b, the minimum of v(A) and v(B) */
std::pair<bundle_t, bundle_t> mu(const bundle_t& b, const Valuation& valuation);

/* mu(b1, b2, v) = mu(b1 | b2, v) */
std::pair<bundle_t, bundle_t> mu(const bundle_t& b1, const bundle_t& b2,
                                 const Valuation& valuation);

/* eta(X, v) is the min over A, B partitions of X, the maximum of v(A) and v(B) */
std::pair<bundle_t, bundle_t> eta(const bundle_t& b,
                                  const Valuation& valuation);

valuation_t muValue(const bundle_t& b1, const Valuation& valuation);

valuation_t muValue(const bundle_t& b1, const bundle_t& b2,
                    const Valuation& valuation);

bool isEnvious(const bundle_t& b1, const bundle_t& b2,
               const Valuation& valuation);

bool isEnvyFree(const Allocation& allocation,
                const std::vector<Valuation>& valuations);

bool isMmsFeasible(const Valuation& valuation);

/* If at_most = 0, then search for all allocations, otherwise exit early if
     found at_most already. */
std::vector<Allocation> getAllAllocations(
    const std::vector<Valuation>& valuations, const std::size_t& at_most = 0);

std::vector<Allocation> getAllAllocationsPrecomputeMu(
    const std::vector<Valuation>& valuations, const std::size_t& at_most = 0);

/* For given allocation find the maximal value over all i, j of
     \mu_i(X_i + X_j, 2) - v_i(X_i).  */
valuation_t maximalEnvy(const Allocation& allocation,
                        const std::vector<Valuation>& valuations);

/* For given allocation find the maximal value over all i, j of
     \mu_i(X_i + X_j, 2) - v_i(X_i).  */
valuation_t maximalEnvyPrecomputedMu(const Allocation& allocation,
                                     const std::vector<Valuation>& valuations,
                                     valuation_t** precomputed_mu);

valuation_t** precomputeMu(const std::vector<Valuation>& valuations);

valuation_t minimalEnvyOverAllAllocations(
    const std::vector<Valuation>& valuations);
}; /* namespace Pmms */

#endif /* PMMS_H */
