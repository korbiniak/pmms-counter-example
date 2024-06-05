#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "src/bundle.h"
#include "src/valuation.h"

#include "test/test_common.h"

TEST(Valuation, All) {
  Valuation valuation({1, 2, 3, 4, 5, 6});
  EXPECT_EQ(valuation[Bundle::bundle({0})], 1);
  EXPECT_EQ(valuation[Bundle::bundle({1, 3, 5})], 12);
}

TEST(Valuation, Normalize) {
  Valuation valuation({1, 2, 3, 4, 5, 6});
  EXPECT_THAT(valuation.normalize(21).get_v(), ElementsAre(1, 2, 3, 4, 5, 6));
  EXPECT_THAT(valuation.normalize(42).get_v(), ElementsAre(2, 4, 6, 8, 10, 12));
  EXPECT_THAT(
      valuation.normalize(1).get_v(),
      ElementsAre(
          DoubleNear(0.047619, 0.000001), DoubleNear(0.0952381, 0.000001),
          DoubleNear(0.142857, 0.000001), DoubleNear(0.190476, 0.000001),
          DoubleNear(0.238095, 0.000001), DoubleNear(0.285714, 0.000001)));
}
