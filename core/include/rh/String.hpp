#pragma once
#define _RHLIB_INCLUDED_STRING

#include <rh.hpp>

_RHLIB_BEGIN

class StringView;
class String;

_RHLIB_HIDDEN_BEGIN

struct StringUtils {
  using size_type = size_t;
  using ssize_type = ssize_t;

  template <typename CharT, typename = enable_if<is_char_type<CharT>>>
  static constexpr size_type length(const CharT* string) {
    size_t result = 0;

    while (*string)
      ++string, ++result;

    return result;
  }

  template <typename CharT, typename = enable_if<is_char_type<CharT>>>
  static constexpr bool isSubStringEqual(const CharT* string, size_type index, const CharT* sub_string, size_type sub_string_length) noexcept {
    for (
      size_type string_index = 0;
      string_index < sub_string_length;
      ++index, ++string_index
    ) {
      if (string[index] != sub_string[string_index])
        return false;
    }

    return true;
  }

  template <typename CharT, typename = enable_if<is_char_type<CharT>>>
  static constexpr ssize_type find(
    const CharT* string, size_type string_length,
    const CharT* sub_string, size_type sub_string_length
  ) noexcept {
    if (sub_string_length == string_length)
      return isSubStringEqual(string, 0, string, sub_string_length) ? 0 : -1;
    else if (sub_string_length > string_length)
      return -1;

    for (size_type search_end = string_length - sub_string_length, base_index = 0; base_index < search_end; ++base_index) {
      if (isSubStringEqual(string, base_index, string, sub_string_length))
        return base_index;
    }

    return -1;
  }

  template <typename CharT, typename = enable_if<is_char_type<CharT>>>
  static constexpr ssize_type find(
    const CharT* string, size_type string_length,
    CharT character
  ) noexcept {
    for (size_type index = 0; index < string_length; ++index) {
      if (string[index] == character)
        return index;
    }

    return -1;
  }
};

_RHLIB_HIDDEN_END

class StringView {
public:
  using type = StringView;

  using char_type = uchar_t;
  using size_type = size_t;
  using ssize_type = ssize_t;

public:
  constexpr StringView() noexcept = default;

  constexpr StringView(const uchar_t* string) noexcept
    : m_string(string) {}

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
  constexpr operator const char_type*() const noexcept {
    return m_string;
  }

public:
  _RHLIB_NODISCARD
  constexpr const char_type* data() const noexcept {
    return m_string;
  }

  _RHLIB_NODISCARD
  constexpr const char_type* begin() const noexcept {
    return m_string;
  }

  _RHLIB_NODISCARD
  constexpr const char_type* end() const noexcept {
    return m_string + length();
  }

  _RHLIB_NODISCARD
  constexpr bool is_empty() const noexcept {
    return m_string ? !m_string[0] : true;
  }

  _RHLIB_NODISCARD
  constexpr size_type length() const noexcept {
    return m_string ? _RHLIBH StringUtils::length(m_string) : 0;
  }

  _RHLIB_NODISCARD
  constexpr bool startsWith(const char_type* string, size_type string_length) const noexcept {
    if (string_length > length())
      return false;

    return _RHLIBH StringUtils::isSubStringEqual(m_string, 0, string, string_length);
  }

  _RHLIB_NODISCARD
  constexpr bool startsWith(StringView string) const noexcept {
    return startsWith(string.data(), string.length());
  }

  _RHLIB_NODISCARD
  constexpr bool endsWith(const char_type* string, size_type string_length) const noexcept {
    size_type length = this->length();

    if (string_length > length)
      return false;

    return _RHLIBH StringUtils::isSubStringEqual(m_string, length - string_length, string, string_length);
  }

  _RHLIB_NODISCARD
  constexpr bool endsWith(StringView string) const noexcept {
    return endsWith(string.data(), string.length());
  }
  
  _RHLIB_NODISCARD
  constexpr ssize_type find(const char_type* string, size_type string_length) const noexcept {
    return _RHLIBH StringUtils::find(m_string, length(), string, string_length);
  }
  
  _RHLIB_NODISCARD
  constexpr ssize_type find(char_type character) const noexcept {
    return _RHLIBH StringUtils::find(m_string, length(), character);
  }

  _RHLIB_NODISCARD
  constexpr ssize_type find(StringView string) const noexcept {
    return find(string.data(), string.length());
  }

  _RHLIB_NODISCARD
  constexpr bool contains(const char_type* string, size_type string_length) const noexcept {
    return find(string, string_length) != -1;
  }

  _RHLIB_NODISCARD
  constexpr bool contains(char_type character) const noexcept {
    return find(character) != -1;
  }

  _RHLIB_NODISCARD
  constexpr bool contains(StringView string) const noexcept {
    return contains(string.data(), string.length());
  }

public:
  _RHLIB_NODISCARD
  constexpr bool operator==(StringView string) const noexcept {
    return _RHLIBH StringUtils::isSubStringEqual(m_string, 0, string.data(), string.length());
  }

  _RHLIB_NODISCARD
  constexpr bool operator!=(StringView string) const noexcept {
    return !operator==(string);
  }

  _RHLIB_NODISCARD
  constexpr char_type operator[](size_type index) const noexcept {
    return m_string[index];
  }

private:
  const uchar_t* m_string = U"";
};

class String {
public:
  using type = String;

  using char_type = uchar_t;
  using size_type = size_t;
  using ssize_type = ssize_t;

public:
  constexpr String() noexcept = default;

  constexpr String(StringView string)
    : String(string.data(), string.length()) {}

  constexpr String(const char_type* string, size_type string_length_in_chars)
    : String()
  {
    _needAllocated(string_length_in_chars + 1);

    for (size_type index = 0; index < string_length_in_chars; ++index)
      m_buffer[index] = string[index];

    m_buffer[string_length_in_chars] = 0;
  }

  constexpr String(size_type count, char_type character = 0)
    : String()
  {
    _needAllocated(count);

    for (size_type i = 0; i < count; ++i)
      m_buffer[i] = character;
  }

  constexpr String(String const& other) noexcept : String() {
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
    size_type length = other.length();

    _needAllocated(length + 1);

    for (size_type i = 0; i < length; ++i)
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

  constexpr operator const char_type*() const noexcept {
    return m_buffer;
  }

public:
  constexpr char_type* data() noexcept {
    return m_buffer;
  }

  constexpr const char_type* data() const noexcept {
    return m_buffer;
  }

  constexpr char_type* begin() noexcept {
    return m_buffer;
  }

  constexpr const char_type* begin() const noexcept {
    return m_buffer;
  }

  constexpr char_type* end() noexcept {
    return m_buffer + length();
  }

  constexpr const char_type* end() const noexcept {
    return m_buffer + length();
  }

  constexpr bool is_empty() const noexcept {
    return m_buffer ? !m_buffer[0] : true;
  }

  constexpr size_type length() const noexcept {
    return m_buffer ? _RHLIBH StringUtils::length(m_buffer) : 0;
  }

  constexpr void reserve(size_type count) {
    _needAllocated(count);
  }

  constexpr size_type capacity() const noexcept {
    return m_allocated;
  }

  constexpr void shrinkToFit() {
    size_type minimal = length() + 1;
    if (m_allocated > minimal)
      _reallocate(minimal);
  }

  constexpr void clear() noexcept {
    if (m_buffer)
      m_buffer[0] = 0;
  }

  constexpr String& insert(size_type index, size_type count, char_type character) {
    size_type length = this->length();

    _needAllocated(length + count + 1);
    _moveRight(index, length - index, count);

    for (size_type i = 0; i < count; ++i, ++index)
      m_buffer[index] = character;

    length += count;
    m_buffer[length] = 0;

    return *this;
  }

  constexpr String& insert(size_type index, char_type character) {
    return insert(index, 1, character);
  }

  constexpr String& insert(size_type index, const char_type* string, size_type string_length) {
    size_type length = this->length();

    _needAllocated(length + string_length + 1);
    _moveRight(index, length - index, string_length);

    for (size_type i = 0; i < string_length; ++i, ++index)
      m_buffer[index] = string[i];

    length += string_length;
    m_buffer[length] = 0;

    return *this;
  }

  constexpr String& insert(size_type index, StringView string) {
    return insert(index, string.data(), string.length());
  }

  constexpr String& erase(size_type index, size_type count) noexcept {
    size_type length = this->length();

    if (index >= length)
      return *this;

    if (index + count > length)
      return *this;

    _moveLeft(index + count, length - (index + count), count);

    length -= count;
    m_buffer[length] = 0;

    return *this;
  }

  constexpr String& append(size_type count, char_type character) {
    size_type length = this->length();

    _needAllocated(length + count + 1);

    for (size_type counter = 0, index = length; counter < count; ++counter, ++index)
      m_buffer[index] = character;

    length += count;
    m_buffer[length] = 0;

    return *this;
  }

  constexpr String& append(char_type character) {
    return append(1, character);
  }

  constexpr String& append(const char_type* string, size_type string_length_in_characters) {
    size_type length = this->length();

    _needAllocated(length + string_length_in_characters + 1);

    for (size_type i = 0, index = length; i < string_length_in_characters; ++i, ++index)
      m_buffer[index] = string[i];

    length += string_length_in_characters;
    m_buffer[length] = 0;

    return *this;
  }

  constexpr String& append(StringView string) noexcept {
    return append(string.data(), string.length());
  }

  constexpr void push(char_type character) {
    size_type length = this->length();

    _needAllocated(length + 1 + 1);

    m_buffer[length] = character;
    m_buffer[length + 1] = 0;
  }

  constexpr char_type pop() noexcept {
    size_type length = this->length();

    if (length == 0)
      return 0;

    char_type character = m_buffer[--length];
    m_buffer[length] = 0;

    return character;
  }

  constexpr bool startsWith(const char_type* string, size_type string_length) const noexcept {
    if (string_length > length())
      return false;

    return _RHLIBH StringUtils::isSubStringEqual(m_buffer, 0, string, string_length);
  }

  constexpr bool startsWith(StringView string) const noexcept {
    return startsWith(string.data(), string.length());
  }

  constexpr bool endsWith(const char_type* string, size_type string_length) const noexcept {
    size_type length = this->length();

    if (string_length > length)
      return false;

    return _RHLIBH StringUtils::isSubStringEqual(m_buffer, length - string_length, string, string_length);
  }

  constexpr bool endsWith(StringView string) const noexcept {
    return endsWith(string.data(), string.length());
  }
  
  constexpr ssize_type find(const char_type* string, size_type string_length) const noexcept {
    return _RHLIBH StringUtils::find(m_buffer, length(), string, string_length);
  }
  
  constexpr ssize_type find(char_type character) const noexcept {
    return _RHLIBH StringUtils::find(m_buffer, length(), character);
  }

  constexpr ssize_type find(StringView string) const noexcept {
    return find(string.data(), string.length());
  }

  constexpr bool contains(const char_type* string, size_type string_length) const noexcept {
    return find(string, string_length) != -1;
  }

  constexpr bool contains(char_type character) const noexcept {
    return find(character) != -1;
  }

  constexpr bool contains(StringView string) const noexcept {
    return contains(string.data(), string.length());
  }

public:
  constexpr bool operator==(StringView string) const noexcept {
    return _RHLIBH StringUtils::isSubStringEqual(m_buffer, 0, string.data(), string.length());
  }

  constexpr bool operator!=(StringView string) const noexcept {
    return !operator==(string);
  }

  constexpr char_type& operator[](size_type index) noexcept {
    return m_buffer[index];
  }

  constexpr char_type operator[](size_type index) const noexcept {
    return m_buffer[index];
  }

private:
  constexpr void _stealOther(String& other) noexcept {
    m_buffer = other.m_buffer;
    m_allocated = other.m_allocated;
    other.m_buffer = nullptr;
    other.m_allocated = 0;
  }

  constexpr void _needAllocated(size_type size) {
    if (m_allocated < size)
      _grow(size);
  }

  constexpr void _grow(size_type min_size) {
    size_type want_allocate = m_allocated + m_allocated / 2;

    if (want_allocate < min_size)
      want_allocate = min_size;

    _reallocate(want_allocate);
  }

  constexpr void _reallocate(size_type new_size) {
    char_type* prev_buffer = m_buffer;
    size_type prev_allocated = m_allocated;

    m_allocated = new_size;
    m_buffer = nullptr;

    if (new_size > 0) {
      m_buffer = new char_type[new_size];
      
      for (size_type i = 0; i < m_allocated; ++i)
        m_buffer[i] = 0;

      if (prev_buffer) {
        for (size_type i = 0; i < min(m_allocated, prev_allocated); ++i)
          m_buffer[i] = prev_buffer[i];
      }
    }

    if (prev_buffer)
      delete[] prev_buffer;
  }

  constexpr void _moveRight(size_type index, size_type count, size_type amount) noexcept {
    char_type* rbegin = m_buffer + index + count - 1;
    char_type* rend = m_buffer + index - 1;

    for (char_type* it = rbegin; it != rend; --it)
      it[amount] = it[0];
  }

  constexpr void _moveLeft(size_type index, size_type count, int amount) noexcept {
    char_type* begin = m_buffer + index;
    char_type* end = m_buffer + index + count;

    for (char_type* it = begin; it != end; ++it)
      it[-amount] = it[0];
  }

private:
  char_type* m_buffer   = nullptr;
  size_type m_allocated = 0;
};

_RHLIB_END

_RHLIB_GLOBAL_CLASS(StringView)
_RHLIB_GLOBAL_CLASS(String)
