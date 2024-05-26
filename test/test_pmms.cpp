#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "src/bundle.h"
#include "src/pmms.h"
#include "src/valuation.h"

TEST(Pmms, Mu) {
  bundle_t b1 = Bundle::bundle({0, 1, 2});
  bundle_t b2 = Bundle::bundle({3, 4, 5});

  Valuation valuation1({1, 2, 3, 4, 5, 6});
  Valuation valuation2({1, 2, 3, 4, 6, 4});

  std::pair<bundle_t, bundle_t> division = Pmms::mu(b1, b2, valuation1);

  EXPECT_THAT(Bundle::toItemsList(division.first, 6),
              ::testing::ElementsAre(3, 5));
  EXPECT_THAT(Bundle::toItemsList(division.second, 6),
              ::testing::ElementsAre(0, 1, 2, 4));

  division = Pmms::mu(b1, b2, valuation2);
  EXPECT_THAT(Bundle::toItemsList(division.first, 6),
              ::testing::ElementsAre(4, 5));
  EXPECT_THAT(Bundle::toItemsList(division.second, 6),
              ::testing::ElementsAre(0, 1, 2, 3));
}

TEST(Pmms, MuValue) {
  bundle_t b1 = Bundle::bundle({0, 1, 2});
  bundle_t b2 = Bundle::bundle({3, 4, 5});

  Valuation valuation1({1, 2, 3, 4, 5, 6});
  Valuation valuation2({1, 2, 3, 4, 6, 4});

  valuation_t value1 = Pmms::muValue(b1, b2, valuation1);
  valuation_t value2 = Pmms::muValue(b1, b2, valuation2);
  EXPECT_EQ(value1, 10);
  EXPECT_EQ(value2, 10);
}

TEST(Pmms, isEnvy) {
  bundle_t b1 = Bundle::bundle({0, 1, 2});
  bundle_t b2 = Bundle::bundle({3, 4, 5});

  Valuation valuation1({1, 2, 3, 4, 5, 6});
  Valuation valuation2({1, 2, 3, 4, 6, 4});

  EXPECT_TRUE(Pmms::isEnvious(b1, b2, valuation1));
  EXPECT_TRUE(Pmms::isEnvious(b1, b2, valuation2));
  EXPECT_FALSE(Pmms::isEnvious(b2, b1, valuation1));
  EXPECT_FALSE(Pmms::isEnvious(b2, b1, valuation2));
}
