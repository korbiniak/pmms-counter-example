#include "gmock/gmock.h"
#include "src/allocation.h"

#include <fstream>
#include <vector>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "test/test_common.h"

TEST(Allocation, iter3) {
  std::stringstream ss;

  std::size_t cnt = 0;
  const std::string expected =
      TestCommon::readFile("test/allocation-iter3.test");
  Allocation::iter3(3, [&](const Allocation& allocation) {
    allocation.dump(ss);
    return (++cnt <= 3);
  });

  EXPECT_EQ(ss.str(), expected);
}

TEST(Allocation, iter_n) {
  std::stringstream ss;

  std::size_t cnt = 0;
  const std::string expected =
      TestCommon::readFile("test/allocation-iter3.test");
  Allocation::iter_n(3, 3, [&](const Allocation& allocation) {
    allocation.dump(ss);
    return (++cnt <= 3);
  });

  EXPECT_EQ(ss.str(), expected);
}

TEST(Allocation, valuationMatrix) {
  Allocation allocation(
      {Bundle::bundle({0, 1}), Bundle::bundle({2}), Bundle::bundle({})}, 3);

  std::vector<Valuation> valuations = {
      Valuation({1, 2, 3}), Valuation({4, 5, 6}), Valuation({7, 8, 9})};

  std::vector<std::vector<valuation_t>> matrix =
      allocation.valuationMatrix(valuations);

  EXPECT_THAT(matrix,
              ::testing::ElementsAre(std::vector<valuation_t>({3, 3, 0}),
                                     std::vector<valuation_t>({9, 6, 0}),
                                     std::vector<valuation_t>({15, 9, 0})));
}
