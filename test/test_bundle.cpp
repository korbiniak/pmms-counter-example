#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "src/bundle.h"

TEST(Bundle, All) {
  bundle_t b1 = Bundle::bundle({0, 1, 2});
  bundle_t b2 = Bundle::bundle({3, 4, 5});
  bundle_t b3 = Bundle::bundle({0, 2, 4});

  EXPECT_EQ(b1, 0b00000111);
  EXPECT_EQ(b2, 0b00111000);
  EXPECT_EQ(b3, 0b00010101);

  EXPECT_EQ(Bundle::bundle(Bundle::toItemsList(b1, 6)), b1);
  EXPECT_EQ(Bundle::bundle(Bundle::toItemsList(b2, 6)), b2);
  EXPECT_EQ(Bundle::bundle(Bundle::toItemsList(b3, 6)), b3);

  EXPECT_EQ(Bundle::addItem(b1, 3), 0b1111);
  EXPECT_EQ(Bundle::addItem(b1, 3), 0b1111);
}
