#include <rh/String.hpp>

using rh::internal::StrImpl::Encoding;

constexpr uint32_t ACCEPT = 0;
constexpr uint32_t REJECT = 1;

// clang-format off
constexpr uint8_t utf8d[] = {
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 00..1f
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 20..3f
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 40..5f
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 60..7f
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9, // 80..9f
  7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7, // a0..bf
  8,8,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2, // c0..df
  0xa,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x4,0x3,0x3, // e0..ef
  0xb,0x6,0x6,0x6,0x5,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8, // f0..ff
  0x0,0x1,0x2,0x3,0x5,0x8,0x7,0x1,0x1,0x1,0x4,0x6,0x1,0x1,0x1,0x1, // s0..s0
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,0,1,0,1,1,1,1,1,1, // s1..s2
  1,2,1,1,1,1,1,2,1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1,1,1,1,1,1,1,1, // s3..s4
  1,2,1,1,1,1,1,1,1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,3,1,3,1,1,1,1,1,1, // s5..s6
  1,3,1,1,1,1,1,3,1,3,1,1,1,1,1,1,1,3,1,1,1,1,1,1,1,1,1,1,1,1,1,1, // s7..s8
};
// clang-format on

static forceinline uint32_t
Utf8DecodeStep(uint32_t* state, unicode_t* codep, uint32_t byte) noexcept {
  uint32_t type = utf8d[byte];
  uint32_t st = *state;

  if (codep != nullptr) {
    *codep = (st != 0) ? (byte & 0x3fu) | (*codep << 6) : (0xff >> type) & (byte);
  }

  return *state = utf8d[256 + st * 16 + type];
}

bool rh::IsUtf8(const char* string) {
  uint32_t state = 0;

  for (; *string; ++string)
    if (Utf8DecodeStep(&state, nullptr, *string) == REJECT)
      return false;

  return true;
}

template <>
size_t Encoding<char8_t>::UnicodeStringLength(
  const char8_t* string,
  size_t lengthInChars
) noexcept {
  uint32_t state = 0;
  size_t result = 0;

  bool hasLength = lengthInChars != 0;

  for (size_t i = 0; hasLength ? i < lengthInChars : string[i]; ++i) {
    Utf8DecodeStep(&state, nullptr, string[i]);

    if (state == ACCEPT || state == REJECT) {
      ++result;
      state = 0;
    }
  }

  return result;
}

template <>
unicode_t Encoding<char8_t>::ToUnicode(const char8_t*& charPtr) noexcept {
  uint32_t state = 0;
  unicode_t result = 0;

  for (; *charPtr; ++charPtr) {
    switch (Utf8DecodeStep(&state, &result, *charPtr)) {
      case ACCEPT:
        ++charPtr;
        return result;
      case REJECT:
        if (!*charPtr)
          return 0;
        ++charPtr;
        return 0xFFFD;
    }
  }

  return 0;
}

template <>
size_t Encoding<char8_t>::FromUnicode(unicode_t codepoint, char8_t* buffer) noexcept {
  if (codepoint <= 0x7F) {
    buffer[0] = static_cast<char8_t>(codepoint);
    return 1;
  }
  else if (codepoint <= 0x7FF) {
    buffer[0] = (codepoint >> 6) | 0xC0;
    buffer[1] = (codepoint & 0x3F) | 0x80;
    return 2;
  }
  else if (codepoint <= 0xFFFF) {
    buffer[0] = (codepoint >> 12) | 0xE0;
    buffer[1] = ((codepoint >> 6) & 0x3F) | 0x80;
    buffer[2] = (codepoint & 0x3F) | 0x80;
    return 3;
  }
  else if (codepoint <= 0x10FFFF) {
    buffer[0] = (codepoint >> 18) | 0xF0;
    buffer[1] = ((codepoint >> 12) & 0x3F) | 0x80;
    buffer[2] = ((codepoint >> 6) & 0x3F) | 0x80;
    buffer[3] = (codepoint & 0x3F) | 0x80;
    return 4;
  }

  return 0;
}
