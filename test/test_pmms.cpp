#include "src/pmms.h"

#include <string>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "src/bundle.h"
#include "src/valuation.h"
#include "test/test_common.h"

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

TEST(Pmms, isEnvyFree) {
  bundle_t b1 = Bundle::bundle({0, 1, 2});
  bundle_t b2 = Bundle::bundle({
      6,
  });
  bundle_t b3 = Bundle::bundle({3, 4, 5});

  Valuation valuation1({35, 21, 21, 21, 20, 35, 10});
  Valuation valuation2({60, 50, 50, 10, 30, 10, 100});
  Valuation valuation3({10, 10, 10, 60, 10, 60, 100});

  Allocation allocation1({b1, b2, b3}, 7);

  allocation1.debugCheckIntegrity();
  EXPECT_TRUE(
      Pmms::isEnvyFree(allocation1, {valuation1, valuation2, valuation3}));
  EXPECT_FALSE(
      Pmms::isEnvyFree(allocation1, {valuation1, valuation3, valuation2}));
}

TEST(Pmms, getAllAllocations) {
  Valuation valuation1({1, 1, 1});
  Valuation valuation2({1, 1, 1});
  Valuation valuation3({1, 1, 1});

  std::stringstream ss;

  std::vector<Allocation> allocations =
      Pmms::getAllAllocations({valuation1, valuation2, valuation3});

  for (uint i = 0; i < allocations.size(); i++) {
    allocations[i].dump(ss);
  }

  std::string expected = R"(0: {2, }
1: {1, }
2: {0, }
0: {2, }
1: {0, }
2: {1, }
0: {1, }
1: {2, }
2: {0, }
0: {1, }
1: {0, }
2: {2, }
0: {0, }
1: {2, }
2: {1, }
0: {0, }
1: {1, }
2: {2, }
)";
  EXPECT_EQ(ss.str(), expected);
}

TEST(Pmms, getAllAllocations2) {
  Valuation valuation1({35, 21, 21, 21, 20, 35, 10});
  Valuation valuation2({60, 50, 50, 10, 30, 10, 100});
  Valuation valuation3({10, 10, 10, 60, 10, 60, 100});

  std::stringstream ss;
  const std::string expected =
      TestCommon::readFile("test/pmms-get-all-allocations.test");

  std::vector<Allocation> allocations =
      Pmms::getAllAllocations({valuation1, valuation2, valuation3});

  for (uint i = 0; i < allocations.size(); i++) {
    allocations[i].dump(ss);
  }

  EXPECT_EQ(ss.str(), expected);
}

TEST(PMMS, maximalEnvy) {
  Valuation valuation1({35, 21, 21, 21, 20, 35, 10});
  Valuation valuation2({60, 50, 50, 10, 30, 10, 100});
  Valuation valuation3({10, 10, 10, 60, 10, 60, 100});

  std::stringstream ss;
  const std::string expected =
      TestCommon::readFile("test/pmms-maximal-envy.test");

  Allocation::iter3(7, [&](const Allocation& allocation) {
    ss << "Allocation:\n";
    allocation.dump(ss);
    ss << Pmms::maximalEnvy(allocation, {valuation1, valuation2, valuation3})
       << std::endl;
  });

  EXPECT_EQ(ss.str(), expected);
}
