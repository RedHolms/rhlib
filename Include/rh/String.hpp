#pragma once

#include <fmt/core.h>
#include <rh/hash.hpp>
#include <rh/types.hpp>
#include <string>

/**
 * Unicode codepoint
 */
using unicode_t = uint32_t;

/**
 * Character types that explicitly specify the encoding
 */
template <typename T>
concept EncodedChar = rh::is_any_of_types<T, char8_t, char16_t, char32_t, wchar_t>;

template <typename T>
constexpr size_t stringLength(const T* string) {
  size_t result = 0;
  for (; *string; ++string)
    ++result;
  return result;
}

namespace rh {

bool IsUtf8(const char* string);

} // namespace rh

namespace rh::internal::StrImpl {

template <EncodedChar ChT>
struct Encoding {
  /**
   * Returns count of Unicode codepoints in given string. If "lengthInChars" is zero, string must be
   * null-terminated. Returns -1 on errors.
   */
  static size_t UnicodeStringLength(const ChT* string, size_t lengthInChars = 0) noexcept;

  /**
   * Decodes single codepoint from a given pointer. Pointer is adjusted to point to the next
   * character. If "charPtr" pointer to the end of the string (null-terminator), then 0 is returned
   * and pointer is not adjusted. If "charPtr" points to an invalid sequence, 0xFFFD is retuned.
   */
  static unicode_t ToUnicode(const ChT*& charPtr) noexcept;

  /**
   * Encodes codepoint to "buffer". Length of the encoded character is returned. Buffer must be able
   * to hold up to 4 (max encoded length of UTF-8) values of type "ChT". If encoding has failed, 0
   * is returned and buffer is not touched.
   */
  static size_t FromUnicode(unicode_t codepoint, ChT* buffer) noexcept;
};

} // namespace rh::internal::StrImpl

/**
 * UTF-32 string
 */
class String {
public:
  constexpr String() = default;

  template <EncodedChar T>
  inline String(const T* rawString, size_t lengthInChars) : String() {
    if constexpr (std::is_same_v<T, char32_t>) {
      _reallocate(lengthInChars + 1);
      memcpy(m_buffer, rawString, lengthInChars * sizeof(char32_t));
      m_buffer[m_length = lengthInChars] = 0;
    }
    else {
      _initFromEncoded(rawString, lengthInChars);
    }
  }

  template <EncodedChar T>
  inline implicit String(const T* rawString) : String(rawString, stringLength(rawString)) {}

  template <EncodedChar T>
  inline implicit String(std::basic_string<T> const& string)
    : String(string.data(), string.length()) {}

  template <EncodedChar T>
  inline implicit String(std::basic_string_view<T> const& string)
    : String(string.data(), string.length()) {}

  constexpr String(String&& other) noexcept {
    _steal(other);
  }

  inline String& operator=(String&& other) noexcept {
    _reset();
    _steal(other);
    return *this;
  }

  inline String(String const& other) noexcept : String() {
    if (other.empty())
      return;

    *this = other;
  }

  inline String& operator=(String const& other) noexcept {
    clear();

    _wantWrite(other.m_length);
    memcpy(m_buffer, other.m_buffer, (m_length = other.m_length) * sizeof(char32_t));
    m_buffer[m_length] = 0;

    return *this;
  }

  inline ~String() {
    _reset();
  }

private:
  inline String(const char* ansiString, size_t lengthInBytes, uint32_t codePage) : String() {
    _initFromANSI(ansiString, lengthInBytes, codePage);
  }

public:
  static String FromANSI(const char* string, size_t lengthInBytes, uint32_t codePage = 0) {
    return String(string, lengthInBytes, codePage);
  }

  static String FromANSI(std::string_view string, uint32_t codePage = 0) {
    return String(string.data(), string.length(), codePage);
  }

  static String FromUTF8(const char* string, size_t lengthInBytes) {
    return String(ptr_cast<const char8_t*>(string), lengthInBytes);
  }

  static String FromUTF8(std::string_view string) {
    return String(ptr_cast<const char8_t*>(string.data()), string.length());
  }

public:
  constexpr char32_t* begin() noexcept {
    return m_buffer;
  }

  constexpr char32_t const* begin() const noexcept {
    return m_buffer;
  }

  constexpr char32_t* end() noexcept {
    return m_buffer + m_length;
  }

  constexpr char32_t const* end() const noexcept {
    return m_buffer + m_length;
  }

  constexpr bool empty() const noexcept {
    return !m_length;
  }

  constexpr size_t length() const noexcept {
    return m_length;
  }

  constexpr size_t allocated() const noexcept {
    return m_allocated;
  }

  constexpr char32_t* data() noexcept {
    return m_buffer;
  }

  constexpr const char32_t* data() const noexcept {
    return m_buffer;
  }

  constexpr const char32_t* c_str() const noexcept {
    return m_buffer;
  }

  constexpr void clear() noexcept {
    if (m_buffer) {
      m_buffer[0] = 0;
      m_length = 0;
    }
  }

  bool contains(String const& substring) const;

  int operator<=>(String const& other) const noexcept;

  forceinline bool operator==(String const& other) const noexcept {
    return (*this <=> other) == 0;
  }
  forceinline bool operator!=(String const& other) const noexcept {
    return (*this <=> other) != 0;
  }
  forceinline bool operator>(String const& other) const noexcept {
    return (*this <=> other) == 1;
  }
  forceinline bool operator<(String const& other) const noexcept {
    return (*this <=> other) == -1;
  }
  forceinline bool operator>=(String const& other) const noexcept {
    return (*this <=> other) != -1;
  }
  forceinline bool operator<=(String const& other) const noexcept {
    return (*this <=> other) != 1;
  }

  constexpr char32_t operator[](size_t index) const noexcept {
    return m_buffer[index];
  }
  constexpr char32_t& operator[](size_t index) noexcept {
    return m_buffer[index];
  }

public:
  template <EncodedChar T>
  inline std::basic_string<T> toSTL() const {
    return _encode<T, T>();
  }

  inline std::string toUTF8() const {
    return _encode<char, char8_t>();
  }

public:
  std::string toANSI(int codePage = 0) const;

private:
  void _initFromANSI(const char* ansiString, size_t lengthInBytes, uint32_t codePage);

  template <EncodedChar ChT>
  inline void _initFromEncoded(const ChT* rawString, size_t lengthInChars) {
    using Enc = rh::internal::StrImpl::Encoding<ChT>;

    auto uniLength = Enc::UnicodeStringLength(rawString, lengthInChars);
    _reallocate(uniLength + 1);

    char32_t codep;

    auto p = rawString;
    while (p - rawString < static_cast<ptrdiff_t>(lengthInChars) && ((codep = Enc::ToUnicode(p))))
      m_buffer[m_length++] = codep;

    m_buffer[m_length] = 0;
  }

  template <typename TrgtChT, EncodedChar EncChT>
    requires(sizeof(TrgtChT) == sizeof(EncChT))
  forceinline std::basic_string<TrgtChT> _encode() const noexcept {
    if (empty())
      return {};

    using Enc = rh::internal::StrImpl::Encoding<EncChT>;

    std::basic_string<TrgtChT> result;
    result.reserve(m_length * 4);

    EncChT buffer[4];

    for (auto codepoint : *this) {
      size_t count = Enc::FromUnicode(codepoint, buffer);

      if (count == 0)
        continue;

      for (size_t i = 0; i < count; ++i)
        result.push_back(static_cast<TrgtChT>(buffer[i]));
    }

    return result;
  }

  inline void _reset() {
    if (m_buffer)
      delete[] m_buffer;

    m_buffer = nullptr;
    m_length = m_allocated = 0;
  }

  constexpr void _steal(String& other) {
    m_buffer = other.m_buffer;
    m_length = other.m_length;
    m_allocated = other.m_allocated;
    other.m_buffer = nullptr;
    other.m_length = other.m_allocated = 0;
  }

  void _wantWrite(size_t charsCount);
  void _reallocate(size_t newSize);

private:
  char32_t* m_buffer = nullptr;
  size_t m_length = 0;    // IN DWORDS
  size_t m_allocated = 0; // IN DWORDS
};

template <>
struct fmt::formatter<String> : formatter<std::string_view> {
  auto format(String const& string, format_context& ctx) const {
    auto encoded = string.toUTF8();
    return formatter<std::string_view>::format(encoded, ctx);
  }
};

template <>
struct std::hash<String> {
  constexpr size_t operator()(String const& string) const noexcept {
    return rh::hash::fnv::v1a_appendBytes(string.data(), string.length() * sizeof(char32_t));
  }
};
