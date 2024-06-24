#include "src/generator.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "test/test_common.h"

TEST(Generator, determinancy) {
  Generator gen1(1);
  Generator gen2(1);

  std::vector<Valuation> valuations1 = gen1.additiveValuations(10, 12, 1, 100);
  std::vector<Valuation> valuations2 = gen2.additiveValuations(10, 12, 1, 100);

  EXPECT_EQ(valuations1, valuations2);
}
