#include <gtest/gtest.h>

#include <rh/String.hpp>

TEST(CoreTests, StringView) {
  // Constructors
  StringView empty;
  StringView fromPtr = U"Hello World!";
  StringView fromStlView = std::u32string_view{U"Hello, World!"};
  StringView fromStlStr = std::u32string{U"Hello, World!"};
  StringView fromOtherView = fromPtr;
  StringView moved = U"Hello, World!";
  StringView fromMoved = rh::move(moved);

  // operator=
  StringView equaled, anotherMoved;
  equaled = U"Hello World!";
  equaled = std::u32string_view{U"Hello, World!"};
  equaled = std::u32string{U"Hello, World!"};
  equaled = fromPtr;
  anotherMoved = U"Hello, World!";
  equaled = rh::move(anotherMoved);

  // Cast operators
  const char32_t* utf32_ptr = empty;
  std::u32string utf32_stl = empty;
  std::u32string_view utf32_stl_view = empty;
  
  // Functions
  EXPECT_EQ(empty.data(), empty.begin());
  EXPECT_EQ(empty.data() + empty.length(), empty.end());
  EXPECT_TRUE(empty.isEmpty());
  EXPECT_TRUE(moved.isEmpty());
  EXPECT_FALSE(fromPtr.isEmpty());
  EXPECT_EQ(fromPtr.length(), 12);
  EXPECT_TRUE(fromPtr.startsWith(U"Hello", 5));
  EXPECT_TRUE(fromPtr.startsWith(U"Hello"));
  EXPECT_TRUE(fromPtr.startsWith(String{U"Hello"}));
  EXPECT_TRUE(fromPtr.startsWith(StringView{U"Hello"}));
}
