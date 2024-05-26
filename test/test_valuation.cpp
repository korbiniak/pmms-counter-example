#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "src/bundle.h"
#include "src/valuation.h"

TEST(Valuation, All) {
  Valuation valuation({1, 2, 3, 4, 5, 6});
  EXPECT_EQ(valuation[Bundle::bundle({0})], 1);
  EXPECT_EQ(valuation[Bundle::bundle({1, 3, 5})], 12);
}
