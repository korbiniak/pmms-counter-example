#include "src/efx.h"
#include "valuation.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

TEST(Efx, isEnvy) {
  Valuation valuation1({50, 50, 100});

  EXPECT_TRUE(
      Efx::isEnvious(Bundle::bundle({0}), Bundle::bundle({1, 2}), valuation1));
  EXPECT_FALSE(
      Efx::isEnvious(Bundle::bundle({0, 1}), Bundle::bundle({2}), valuation1));
  EXPECT_FALSE(
      Efx::isEnvious(Bundle::bundle({0, 1}), Bundle::bundle({2}), valuation1));
  EXPECT_FALSE(
      Efx::isEnvious(Bundle::bundle({0, 1}), Bundle::bundle({}), valuation1));
  EXPECT_FALSE(
      Efx::isEnvious(Bundle::bundle({0}), Bundle::bundle({2}), valuation1));
}
