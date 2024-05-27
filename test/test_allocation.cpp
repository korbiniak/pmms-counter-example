#include "src/allocation.h"

#include <fstream>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "test/test_common.h"

TEST(Allocation, iter3) {
  std::stringstream ss;

  const std::string expected =
      TestCommon::readFile("test/allocation-iter3.out");
  Allocation::iter3(3,
                    [&](const Allocation& allocation) { allocation.dump(ss); });

  EXPECT_EQ(ss.str(), expected);
}
