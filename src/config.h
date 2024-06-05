#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>
#include <limits>

#ifndef VALUATION_T_INT64

using valuation_t = double;
static constexpr valuation_t MIN_VALUATION =
    -std::numeric_limits<double>::max();
static constexpr valuation_t MAX_VALUATION = std::numeric_limits<double>::max();

#else

using valuation_t = int64_t;
static constexpr valuation_t MIN_VALUATION =
    std::numeric_limits<valuation_t>::min();
static constexpr valuation_t MAX_VALUATION =
    std::numeric_limits<valuation_t>::max();
#endif /* VALUATION_T_INT64 */

using bundle_t = uint32_t;

#endif /* CONFIG_H */
