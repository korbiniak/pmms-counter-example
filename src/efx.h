#ifndef EFX_H
#define EFX_H

#include <utility>

#include "src/bundle.h"
#include "src/config.h"
#include "src/valuation.h"

#include "src/allocation.h"

namespace Efx {
bool isEnvious(const bundle_t& b1, const bundle_t& b2,
               const Valuation& valuation);

bool isEnvyFree(const Allocation& allocation,
                const std::vector<Valuation>& valuations);

/* If at_most = 0, then search for all allocations, otherwise exit early if
     found at_most already. */
std::vector<Allocation> getAllAllocations(
    const std::vector<Valuation>& valuations, const std::size_t& at_most = 0);
}; /* namespace Efx */

#endif /* EFX_H */
