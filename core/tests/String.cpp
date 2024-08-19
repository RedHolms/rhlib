#include <gtest/gtest.h>

#include <rh/String.hpp>

TEST(StringTests, Unicode) {
  String first, second;

  EXPECT_TRUE(first.is_empty());
  EXPECT_EQ(first.length(), 0);

  first = U"Hello, World!";

  EXPECT_EQ(first, U"Hello, World!");
  EXPECT_EQ(first.length(), 13);

  second = first;

  EXPECT_EQ(second.length(), 13);
  EXPECT_EQ(first, second);

  {
    char32_t* first_buffer = first.data();
    second = rh::move(first);

    EXPECT_EQ(second.data(), first_buffer);
  }

  first = rh::move(second);

  first.reserve(32);
  EXPECT_EQ(first.capacity(), 32);

  first.reserve(1);
  EXPECT_EQ(first.capacity(), 32);

  first.shrinkToFit();
  EXPECT_EQ(first.capacity(), first.length() + 1);

  {
    size_t prev_capacity = first.capacity();
    first.clear();
    EXPECT_EQ(prev_capacity, first.capacity());
  }

  first = U"World";
  first.insert(0, U"Hello, ");

  EXPECT_EQ(first, U"Hello, World");

  first.append(U'!');
  EXPECT_EQ(first, U"Hello, World!");

  first.erase(first.length() - 1, 1);
  EXPECT_EQ(first, U"Hello, World");

  second = U"Hello";
  EXPECT_TRUE(first.startsWith(U"Hello"));
  EXPECT_TRUE(first.startsWith(second));
  EXPECT_TRUE(first.startsWith(second.data()));

  second = U"World";
  EXPECT_TRUE(first.endsWith(U"World"));
  EXPECT_TRUE(first.endsWith(second));
  EXPECT_TRUE(first.endsWith(second.data()));

  second = U"llo, Wo";
  EXPECT_TRUE(first.contains(U"llo, Wo"));
  EXPECT_TRUE(first.contains(second));
  EXPECT_TRUE(first.contains(second.data()));

  EXPECT_EQ(first[1], U'e');
}
