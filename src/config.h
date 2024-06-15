#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>
#include <limits>

#ifdef VALUATION_T_INT64

using valuation_t = int64_t;
static constexpr valuation_t MIN_VALUATION =
    std::numeric_limits<valuation_t>::min();
static constexpr valuation_t MAX_VALUATION =
    std::numeric_limits<valuation_t>::max();

#else

using valuation_t = double;
static constexpr valuation_t MIN_VALUATION =
    -std::numeric_limits<double>::max();
static constexpr valuation_t MAX_VALUATION = std::numeric_limits<double>::max();

#endif /* VALUATION_T_INT64 */

using bundle_t = uint32_t;

template <typename T>
T** create2DArray(const std::size_t& n, const std::size_t& m) {
  T* data = new T[n * m];
  T** array = new T*[n];

  for (std::size_t i = 0; i < n; ++i) {
    array[i] = data + i * m;
  }

  return array;
}

template <typename T>
void free2DArray(T** array) {
  delete[] array[0];
  delete[] array;
}

#endif /* CONFIG_H */
