#include <gtest/gtest.h>

#include <rh/String.hpp>

TEST(StringTests, DefaultConstruction) {
  String string;
  EXPECT_TRUE(string.empty());
  EXPECT_EQ(string.data(), nullptr);
  EXPECT_EQ(string.length(), 0);
  EXPECT_EQ(string.allocated(), 0);
}
