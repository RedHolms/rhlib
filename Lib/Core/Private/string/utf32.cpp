#include <rh/String.hpp>

using rh::internal::StrImpl::Encoding;

template <>
size_t Encoding<char32_t>::UnicodeStringLength(
  const char32_t* string,
  size_t lengthInChars
) noexcept {
  return lengthInChars != 0 ? lengthInChars : stringLength(string);
}

template <>
unicode_t Encoding<char32_t>::ToUnicode(const char32_t*& charPtr) noexcept {
  return static_cast<unicode_t>(*charPtr++);
}

template <>
size_t Encoding<char32_t>::FromUnicode(unicode_t codepoint, char32_t* buffer) noexcept {
  buffer[0] = static_cast<char32_t>(codepoint);
  return 1;
}
