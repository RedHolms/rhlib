#include <gtest/gtest.h>

#include <gmock/gmock-matchers.h>
#include <rh/String.hpp>

TEST(StringTests, DefaultConstruction) {
  String string;
  EXPECT_TRUE(string.empty());
  EXPECT_EQ(string.data(), nullptr);
  EXPECT_EQ(string.length(), 0);
  EXPECT_EQ(string.allocated(), 0);
}

#define DEFINE_LITERAL_CONSTRUCTION_TEST(NAME, PREFIX)                                             \
  TEST(StringTests, NAME##LiteralConstruction) {                                                   \
    using testing::ElementsAre;                                                                    \
                                                                                                   \
    String string = PREFIX##"Hello, 美丽 мир!";                                                    \
    EXPECT_FALSE(string.empty());                                                                  \
    EXPECT_EQ(string.length(), 14);                                                                \
    EXPECT_THAT(                                                                                   \
      string,                                                                                      \
      ElementsAre(                                                                                 \
        U'H', U'e', U'l', U'l', U'o', U',', U' ', U'美', U'丽', U' ', U'м', U'и', U'р', U'!'       \
      )                                                                                            \
    );                                                                                             \
  }

DEFINE_LITERAL_CONSTRUCTION_TEST(Utf8, u8)
DEFINE_LITERAL_CONSTRUCTION_TEST(Utf16, u)
DEFINE_LITERAL_CONSTRUCTION_TEST(Wide, L)
DEFINE_LITERAL_CONSTRUCTION_TEST(Utf32, U)
