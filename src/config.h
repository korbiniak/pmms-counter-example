#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>
#include <limits>

using valuation_t = double;
static constexpr valuation_t MIN_VALUATION =
    -std::numeric_limits<double>::max();
static constexpr valuation_t MAX_VALUATION = std::numeric_limits<double>::max();

using bundle_t = uint32_t;

#endif
