#include <rh/String.hpp>

using rh::internal::StrImpl::Encoding;

template <EncodedChar ChT>
struct Utf16Impl {
  static forceinline size_t CharLength(const ChT* str) noexcept {
    uint16_t c = static_cast<uint16_t>(*str);
    return (c >> 10) == 0b110110 ? 2 : 1;
  }

  static forceinline size_t UnicodeStringLength(const ChT* string, size_t lengthInChars) noexcept {
    size_t result = 0;

    bool hasLength = lengthInChars != 0;

    for (size_t i = 0; hasLength ? i < lengthInChars : string[i]; i += CharLength(string))
      ++result;

    return result;
  }

  static forceinline unicode_t ToUnicode(const ChT*& charPtr) noexcept {
    auto l = CharLength(charPtr);
    if (l == 1)
      return static_cast<char32_t>(*charPtr++);

    uint16_t high = static_cast<uint16_t>(*charPtr++) - 0xD800;
    uint16_t low = static_cast<uint16_t>(*charPtr++) - 0xDC00;
    return (high << 10) + low + 0x10000;
  }

  static forceinline size_t FromUnicode(unicode_t codepoint, ChT* buffer) noexcept {
    if (codepoint > 0xFFFF) {
      buffer[0] = 0xD7C0 + (codepoint >> 10);
      buffer[1] = 0xDC00 + (codepoint & 0x3FF);
      return 2;
    }

    buffer[0] = static_cast<ChT>(codepoint);
    return 1;
  }
};

#define DECLARE_UTF16(ChT)                                                                         \
  template <>                                                                                      \
  size_t Encoding<ChT>::UnicodeStringLength(const ChT* string, size_t lengthInChars) noexcept {    \
    return Utf16Impl<ChT>::UnicodeStringLength(string, lengthInChars);                             \
  }                                                                                                \
  template <>                                                                                      \
  unicode_t Encoding<ChT>::ToUnicode(const ChT*& charPtr) noexcept {                               \
    return Utf16Impl<ChT>::ToUnicode(charPtr);                                                     \
  }                                                                                                \
  template <>                                                                                      \
  size_t Encoding<ChT>::FromUnicode(unicode_t codepoint, ChT* buffer) noexcept {                   \
    return Utf16Impl<ChT>::FromUnicode(codepoint, buffer);                                         \
  }

DECLARE_UTF16(char16_t)
DECLARE_UTF16(wchar_t)
