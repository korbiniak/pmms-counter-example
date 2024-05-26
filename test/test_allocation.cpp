#include "src/allocation.h"

#include <fstream>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

std::string readFile(const std::string& filePath) {
  std::ifstream file(filePath);
  std::string content((std::istreambuf_iterator<char>(file)),
                      std::istreambuf_iterator<char>());
  return content;
}

TEST(Allocation, iter3) {
  std::stringstream ss;

  const std::string expected = readFile("test/allocation-iter3-test.out");
  Allocation::iter3(3,
                    [&](const Allocation& allocation) { allocation.dump(ss); });

  EXPECT_EQ(ss.str(), expected);
}
