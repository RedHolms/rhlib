#pragma once
#define _RHLIB_INCLUDED_STRING

#include <rh.hpp>

#include <rh/TypeTraits.hpp>

#ifndef _RHLIB_NO_STL_COMPAT
# include <string>
# include <string_view>
#endif

_RHLIB_BEGIN

class StringView;
class String;

_RHLIB_HIDDEN_BEGIN

struct StringUtils {
  template <typename CharT, typename = enable_if<is_char_type<CharT>>>
  static constexpr size_t length(const CharT* string) {
    size_t result = 0;

    while (*string)
      ++string, ++result;

    return result;
  }

  template <typename CharT, typename = enable_if<is_char_type<CharT>>>
  static constexpr bool isSubStringEqual(const CharT* string, size_t index, const CharT* sub_string, size_t sub_string_length) noexcept {
    for (
      size_t string_index = 0;
      string_index < sub_string_length;
      ++index, ++string_index
    ) {
      if (string[index] != sub_string[string_index])
        return false;
    }

    return true;
  }

  template <typename CharT, typename = enable_if<is_char_type<CharT>>>
  static constexpr ssize_t find(
    const CharT* string, size_t string_length,
    const CharT* sub_string, size_t sub_string_length
  ) noexcept {
    if (sub_string_length == string_length)
      return isSubStringEqual(string, 0, string, sub_string_length) ? 0 : -1;
    else if (sub_string_length > string_length)
      return -1;

    for (size_t search_end = string_length - sub_string_length, base_index = 0; base_index < search_end; ++base_index) {
      if (isSubStringEqual(string, base_index, string, sub_string_length))
        return base_index;
    }

    return -1;
  }

  template <typename CharT, typename = enable_if<is_char_type<CharT>>>
  static constexpr ssize_t find(
    const CharT* string, size_t string_length,
    CharT character
  ) noexcept {
    for (size_t index = 0; index < string_length; ++index) {
      if (string[index] == character)
        return index;
    }

    return -1;
  }
};

_RHLIB_HIDDEN_END

class StringView {
private:
  char32_t const* m_string = U"";

public:
  constexpr StringView() noexcept = default;

  constexpr StringView(const char32_t* string) noexcept
    : m_string(string) {}

#ifndef _RHLIB_NO_STL_COMPAT
  constexpr StringView(std::u32string_view string) noexcept
    : m_string(string.data()) {}

  constexpr StringView(std::u32string const& string) noexcept
    : m_string(string.data()) {}
#endif

  constexpr StringView(StringView const& other) noexcept
    : m_string(other.m_string) {}
    
  constexpr StringView(StringView&& other) noexcept
    : m_string(other.m_string)
  {
    other.m_string = U"";
  }

  constexpr StringView& operator=(StringView const& other) {
    m_string = other.m_string;
    return *this;
  }

  constexpr StringView& operator=(StringView&& other) noexcept {
    m_string = other.m_string;
    other.m_string = U"";
    return *this;
  }

public:
  constexpr operator const char32_t*() const noexcept {
    return m_string;
  }

#ifndef _RHLIB_NO_STL_COMPAT
  constexpr operator std::u32string_view() const noexcept {
    return std::u32string_view{m_string};
  }
  constexpr operator std::u32string() const noexcept {
    return std::u32string{m_string};
  }

#endif

public:
  [[nodiscard]]
  constexpr const char32_t* data() const noexcept {
    return m_string;
  }

  [[nodiscard]]
  constexpr const char32_t* begin() const noexcept {
    return m_string;
  }

  [[nodiscard]]
  constexpr const char32_t* end() const noexcept {
    return m_string + length();
  }

  [[nodiscard]]
  constexpr bool isEmpty() const noexcept {
    return m_string ? !m_string[0] : true;
  }

  [[nodiscard]]
  constexpr size_t length() const noexcept {
    return m_string ? _RHLIBH StringUtils::length(m_string) : 0;
  }

  [[nodiscard]]
  constexpr bool startsWith(const char32_t* string, size_t string_length) const noexcept {
    if (string_length > length())
      return false;

    return _RHLIBH StringUtils::isSubStringEqual(m_string, 0, string, string_length);
  }

  [[nodiscard]]
  constexpr bool startsWith(StringView string) const noexcept {
    return startsWith(string.data(), string.length());
  }

  [[nodiscard]]
  constexpr bool endsWith(const char32_t* string, size_t string_length) const noexcept {
    size_t length = this->length();

    if (string_length > length)
      return false;

    return _RHLIBH StringUtils::isSubStringEqual(m_string, length - string_length, string, string_length);
  }

  [[nodiscard]]
  constexpr bool endsWith(StringView string) const noexcept {
    return endsWith(string.data(), string.length());
  }
  
  [[nodiscard]]
  constexpr ssize_t find(const char32_t* string, size_t string_length) const noexcept {
    return _RHLIBH StringUtils::find(m_string, length(), string, string_length);
  }
  
  [[nodiscard]]
  constexpr ssize_t find(char32_t character) const noexcept {
    return _RHLIBH StringUtils::find(m_string, length(), character);
  }

  [[nodiscard]]
  constexpr ssize_t find(StringView string) const noexcept {
    return find(string.data(), string.length());
  }

  [[nodiscard]]
  constexpr bool contains(const char32_t* string, size_t string_length) const noexcept {
    return find(string, string_length) != -1;
  }

  [[nodiscard]]
  constexpr bool contains(char32_t character) const noexcept {
    return find(character) != -1;
  }

  [[nodiscard]]
  constexpr bool contains(StringView string) const noexcept {
    return contains(string.data(), string.length());
  }

public:
  [[nodiscard]]
  constexpr bool operator==(StringView string) const noexcept {
    return _RHLIBH StringUtils::isSubStringEqual(m_string, 0, string.data(), string.length());
  }

  [[nodiscard]]
  constexpr bool operator!=(StringView string) const noexcept {
    return !operator==(string);
  }

  [[nodiscard]]
  constexpr char32_t operator[](size_t index) const noexcept {
    return m_string[index];
  }
};

class String {
private:
  char32_t* m_buffer    = nullptr;
  size_t    m_allocated = 0;

public:
  constexpr String() noexcept = default;

  constexpr String(StringView string)
    : String(string.data(), string.length()) {}

  constexpr String(const char32_t* string, size_t string_length_in_chars)
    : String()
  {
    _needAllocated(string_length_in_chars + 1);

    for (size_t index = 0; index < string_length_in_chars; ++index)
      m_buffer[index] = string[index];

    m_buffer[string_length_in_chars] = 0;
  }

  constexpr String(size_t count, char32_t character = 0)
    : String()
  {
    _needAllocated(count);

    for (size_t i = 0; i < count; ++i)
      m_buffer[i] = character;
  }

  constexpr String(String const& other) : String() {
    operator=(other);
  }

  constexpr String(String&& other) noexcept {
    _stealOther(other);
  }

  constexpr String& operator=(String const& other) {
    *this = StringView{other};
    return *this;
  }

  constexpr String& operator=(StringView other) {
    size_t length = other.length();

    _needAllocated(length + 1);

    for (size_t i = 0; i < length; ++i)
      m_buffer[i] = other[i];

    m_buffer[length] = 0;

    return *this;
  }

  constexpr String& operator=(String&& other) noexcept {
    String::~String();
    _stealOther(other);
    return *this;
  }

  constexpr ~String() {
    _reallocate(0);
  }

public:
  constexpr operator StringView() const noexcept {
    return StringView(m_buffer);
  }

  constexpr operator const char32_t*() const noexcept {
    return m_buffer;
  }

#ifndef _RHLIB_NO_STL_COMPAT
  constexpr operator std::u32string_view() const noexcept {
    return std::u32string_view{m_buffer};
  }

  constexpr operator std::u32string() const noexcept {
    return std::u32string{m_buffer};
  }
#endif

public:
  [[nodiscard]]
  constexpr char32_t* data() noexcept {
    return m_buffer;
  }

  [[nodiscard]]
  constexpr const char32_t* data() const noexcept {
    return m_buffer;
  }

  [[nodiscard]]
  constexpr char32_t* begin() noexcept {
    return m_buffer;
  }

  [[nodiscard]]
  constexpr const char32_t* begin() const noexcept {
    return m_buffer;
  }

  [[nodiscard]]
  constexpr char32_t* end() noexcept {
    return m_buffer + length();
  }

  [[nodiscard]]
  constexpr const char32_t* end() const noexcept {
    return m_buffer + length();
  }

  [[nodiscard]]
  constexpr bool isEmpty() const noexcept {
    return m_buffer ? !m_buffer[0] : true;
  }

  [[nodiscard]]
  constexpr size_t length() const noexcept {
    return m_buffer ? _RHLIBH StringUtils::length(m_buffer) : 0;
  }

  constexpr void reserve(size_t count) {
    _needAllocated(count);
  }

  [[nodiscard]]
  constexpr size_t capacity() const noexcept {
    return m_allocated;
  }

  constexpr void shrinkToFit() {
    size_t minimal = length() + 1;
    if (m_allocated > minimal)
      _reallocate(minimal);
  }

  constexpr void clear() noexcept {
    if (m_buffer)
      m_buffer[0] = 0;
  }

  constexpr String& insert(size_t index, size_t count, char32_t character) {
    size_t length = this->length();

    _needAllocated(length + count + 1);
    _moveRight(index, length - index, count);

    for (size_t i = 0; i < count; ++i, ++index)
      m_buffer[index] = character;

    length += count;
    m_buffer[length] = 0;

    return *this;
  }

  constexpr String& insert(size_t index, char32_t character) {
    return insert(index, 1, character);
  }

  constexpr String& insert(size_t index, const char32_t* string, size_t string_length) {
    size_t length = this->length();

    _needAllocated(length + string_length + 1);
    _moveRight(index, length - index, string_length);

    for (size_t i = 0; i < string_length; ++i, ++index)
      m_buffer[index] = string[i];

    length += string_length;
    m_buffer[length] = 0;

    return *this;
  }

  constexpr String& insert(size_t index, StringView string) {
    return insert(index, string.data(), string.length());
  }

  constexpr String& erase(size_t index, size_t count = 1) noexcept {
    size_t length = this->length();

    if (index >= length)
      return *this; // FIXME: throw exception

    if (index + count > length)
      return *this; // FIXME: throw exception

    _moveLeft(index + count, length - (index + count), count);

    length -= count;
    m_buffer[length] = 0;

    return *this;
  }

  constexpr String& erase(const char32_t* it, size_t count = 1) noexcept {
    ssize_t index = static_cast<ssize_t>(it - m_buffer);

    if (index < 0)
      return *this; // FIXME: throw exception

    return erase(index, count);
  }

  constexpr String& append(size_t count, char32_t character) {
    size_t length = this->length();

    _needAllocated(length + count + 1);

    for (size_t counter = 0, index = length; counter < count; ++counter, ++index)
      m_buffer[index] = character;

    length += count;
    m_buffer[length] = 0;

    return *this;
  }

  constexpr String& append(char32_t character) {
    return append(1, character);
  }

  constexpr String& append(const char32_t* string, size_t string_length_in_characters) {
    size_t length = this->length();

    _needAllocated(length + string_length_in_characters + 1);

    for (size_t i = 0, index = length; i < string_length_in_characters; ++i, ++index)
      m_buffer[index] = string[i];

    length += string_length_in_characters;
    m_buffer[length] = 0;

    return *this;
  }

  constexpr String& append(StringView string) noexcept {
    return append(string.data(), string.length());
  }

  constexpr void push(char32_t character) {
    size_t length = this->length();

    _needAllocated(length + 1 + 1);

    m_buffer[length] = character;
    m_buffer[length + 1] = 0;
  }

  constexpr char32_t pop() noexcept {
    size_t length = this->length();

    if (length == 0)
      return 0;

    char32_t character = m_buffer[--length];
    m_buffer[length] = 0;

    return character;
  }

  [[nodiscard]]
  constexpr bool startsWith(const char32_t* string, size_t string_length) const noexcept {
    if (string_length > length())
      return false;

    return _RHLIBH StringUtils::isSubStringEqual(m_buffer, 0, string, string_length);
  }

  [[nodiscard]]
  constexpr bool startsWith(StringView string) const noexcept {
    return startsWith(string.data(), string.length());
  }

  [[nodiscard]]
  constexpr bool endsWith(const char32_t* string, size_t string_length) const noexcept {
    size_t length = this->length();

    if (string_length > length)
      return false;

    return _RHLIBH StringUtils::isSubStringEqual(m_buffer, length - string_length, string, string_length);
  }

  [[nodiscard]]
  constexpr bool endsWith(StringView string) const noexcept {
    return endsWith(string.data(), string.length());
  }
  
  [[nodiscard]]
  constexpr ssize_t find(const char32_t* string, size_t string_length) const noexcept {
    return _RHLIBH StringUtils::find(m_buffer, length(), string, string_length);
  }
  
  [[nodiscard]]
  constexpr ssize_t find(char32_t character) const noexcept {
    return _RHLIBH StringUtils::find(m_buffer, length(), character);
  }

  [[nodiscard]]
  constexpr ssize_t find(StringView string) const noexcept {
    return find(string.data(), string.length());
  }

  [[nodiscard]]
  constexpr bool contains(const char32_t* string, size_t string_length) const noexcept {
    return find(string, string_length) != -1;
  }

  [[nodiscard]]
  constexpr bool contains(char32_t character) const noexcept {
    return find(character) != -1;
  }

  [[nodiscard]]
  constexpr bool contains(StringView string) const noexcept {
    return contains(string.data(), string.length());
  }

public:
  [[nodiscard]]
  constexpr bool operator==(String const& string) const noexcept {
    return *this == StringView{string};
  }

  [[nodiscard]]
  constexpr bool operator==(const char32_t* string) const noexcept {
    return *this == StringView{string};
  }

  [[nodiscard]]
  constexpr bool operator==(StringView string) const noexcept {
    return _RHLIBH StringUtils::isSubStringEqual(m_buffer, 0, string.data(), string.length());
  }

  [[nodiscard]]
  constexpr bool operator!=(String const& string) const noexcept {
    return !operator==(string);
  }

  [[nodiscard]]
  constexpr bool operator!=(const char32_t* string) const noexcept {
    return !operator==(string);
  }

  [[nodiscard]]
  constexpr bool operator!=(StringView string) const noexcept {
    return !operator==(string);
  }

  [[nodiscard]]
  constexpr char32_t& operator[](size_t index) noexcept {
    return m_buffer[index];
  }

  [[nodiscard]]
  constexpr char32_t operator[](size_t index) const noexcept {
    return m_buffer[index];
  }

private:
  constexpr void _stealOther(String& other) noexcept {
    m_buffer = other.m_buffer;
    m_allocated = other.m_allocated;
    other.m_buffer = nullptr;
    other.m_allocated = 0;
  }

  constexpr void _needAllocated(size_t size) {
    if (m_allocated < size)
      _grow(size);
  }

  constexpr void _grow(size_t min_size) {
    size_t want_allocate = m_allocated + m_allocated / 2;

    if (want_allocate < min_size)
      want_allocate = min_size;

    _reallocate(want_allocate);
  }

  constexpr void _reallocate(size_t new_size) {
    char32_t* prev_buffer = m_buffer;
    size_t prev_allocated = m_allocated;

    m_allocated = new_size;
    m_buffer = nullptr;

    if (new_size > 0) {
      m_buffer = new char32_t[new_size];
      
      for (size_t i = 0; i < m_allocated; ++i)
        m_buffer[i] = 0;

      if (prev_buffer) {
        for (size_t i = 0; i < min(m_allocated, prev_allocated); ++i)
          m_buffer[i] = prev_buffer[i];
      }
    }

    if (prev_buffer)
      delete[] prev_buffer;
  }

  constexpr void _moveRight(size_t index, size_t count, size_t amount) noexcept {
    char32_t* rbegin = m_buffer + index + count - 1;
    char32_t* rend = m_buffer + index - 1;

    for (char32_t* it = rbegin; it != rend; --it)
      it[amount] = it[0];
  }

  constexpr void _moveLeft(size_t index, size_t count, int amount) noexcept {
    char32_t* begin = m_buffer + index;
    char32_t* end = m_buffer + index + count;

    for (char32_t* it = begin; it != end; ++it)
      it[-amount] = it[0];
  }
};

_RHLIB_END

_RHLIB_GLOBAL_CLASS(StringView);
_RHLIB_GLOBAL_CLASS(String);

constexpr rh::String operator+(rh::StringView left, rh::StringView right) noexcept {
  return rh::String{left}.append(right);
}

constexpr rh::String& operator+=(rh::String& left, rh::StringView right) noexcept {
  return left.append(right);
}
