#ifndef BUNDLE_H
#define BUNDLE_H

#include <vector>

#include "src/config.h"

class Bundle {
 public:
  static inline bundle_t addItem(const bundle_t& bundle, const int& i) {
    return bundle | (1 << i);
  }

  static inline bundle_t removeItem(const bundle_t& bundle, const int& i) {
    return bundle & ~(1 << i);
  }

  static inline bool hasItem(const bundle_t& bundle, const int& i) {
    return bundle & (1 << i);
  }

  static inline bundle_t bundle(const std::vector<int> items) {
    bundle_t res = 0;
    for (auto i : items) {
      res = addItem(res, i);
    }
    return res;
  }

  static inline std::vector<int> toItemsList(const bundle_t& bundle,
                                             const int& m) {
    std::vector<int> result;
    for (int i = 0; i < m; i++) {
      if (hasItem(bundle, i)) {
        result.push_back(i);
      }
    }
    return result;
  }
};

/* WARNING: Note that [continue] might be dangerous. */
#define BUNDLE_LOOP(bundle, available_items, code)  \
  for (bundle_t bundle = (available_items);;        \
       bundle = (bundle - 1) & (available_items)) { \
    {                                               \
      code                                          \
    }                                               \
    if (bundle == 0) {                              \
      break;                                        \
    }                                               \
  }

#endif
