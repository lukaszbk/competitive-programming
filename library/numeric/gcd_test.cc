#include "library/numeric/gcd.h"

#include <gtest/gtest.h>

namespace cpl {

TEST(GcdTest, Trivial) {
  EXPECT_EQ(1, gcd(1, 123));
  EXPECT_EQ(1, gcd(123, 1));

  EXPECT_EQ(123, gcd(123, 123));
  EXPECT_EQ(123, gcd(123, 123));
}

TEST(GcdTest, Coprime) {
  EXPECT_EQ(1, gcd(7, 2));
  EXPECT_EQ(1, gcd(2, 7));

  EXPECT_EQ(1, gcd(9, 4));
  EXPECT_EQ(1, gcd(4, 9));
}

TEST(GcdTest, NotCoprime) {
  EXPECT_EQ(3, gcd(33, 18));
  EXPECT_EQ(3, gcd(18, 33));

  EXPECT_EQ(18, gcd(36, 18));
  EXPECT_EQ(18, gcd(18, 36));
}

}  // namespace cpl