#pragma once
#define _RHLIB_INCLUDED

#ifdef _WINDOWS_
# warning Consider including <Windows.h> after rhlib headers
# undef min
# undef max
#endif

#if __cplusplus < 202302L
# error rhlib requires at least C++23
#endif

#if !defined(__has_extension) || !defined(__has_feature)
# error Invalid compiler
#endif

#define _RHLIB_BEGIN        namespace rh {
#define _RHLIB_END          }

#define _RHLIBH             ::rh::_Hidden::
#define _RHLIB_HIDDEN_BEGIN namespace _Hidden {
#define _RHLIB_HIDDEN_END   }

#ifndef _RHLIB_NO_GLOBALS
# define _RHLIB_GLOBAL_CLASS(clazz)  using rh::clazz
# define _RHLIB_GLOBAL_NS(ns)        namespace ns { using namespace rh::ns; }
#else
# define _RHLIB_GLOBAL_CLASS(...)
# define _RHLIB_GLOBAL_NS(...)
#endif

#define _RHLIB_ALLOCATOR __declspec(allocator)

#define _RHLIB_UNUSED(v) [](...){}(v)

#define _RHLIB_API extern

#define _RHLIB_OS_UNSUPPORTED 0
#define _RHLIB_OS_WINDOWS     1

#if defined(_WIN32)
# define _RHLIB_OS _RHLIB_OS_WINDOWS
#else
# define _RHLIB_OS _RHLIB_OS_UNSUPPORTED
#endif

#if \
  defined(__amd64__)  || defined(__amd64)  || defined(_M_AMD64)    || \
  defined(__x86_64__) || defined(__x86_64) || defined(_M_X64)      || \
  defined(__ia64__)   || defined(_IA64)    || defined(__IA64__)    || \
  defined(__ia64)     || defined(_M_IA64)  || defined(__itanium__)
# define _RHLIB_BITNESS 64
#else
# define _RHLIB_BITNESS 32
#endif

static_assert((_RHLIB_BITNESS & 0b111) == 0, "Invalid _RHLIB_BITNESS");
static_assert(sizeof(void*) == (_RHLIB_BITNESS >> 3), "Invalid void* size. Predicted bitness missmatched");

_RHLIB_BEGIN

enum class OSType {};

static constexpr OSType OS_UNSUPPORTED = static_cast<OSType>(_RHLIB_OS_UNSUPPORTED);
static constexpr OSType OS_WINDOWS     = static_cast<OSType>(_RHLIB_OS_WINDOWS);

static constexpr OSType OS             = static_cast<OSType>(_RHLIB_OS);

static constexpr int BITNESS           = _RHLIB_BITNESS;

template <typename T>
struct type_wrapper {
  using type = T;
};

template <typename T, T Value>
struct value_wrapper {
  static constexpr T value = Value;
};

template <bool Value>
struct boolean_wrapper
  : value_wrapper<bool, Value> {};

template <typename Wrapper>
using unwrap_type = typename Wrapper::type;

template <typename Wrapper>
static constexpr auto unwrap_value = Wrapper::value;

// I really don't recommend to use [u]int8_t, becuase it chat be missmatched with char.
// Use "byte" everywhere you can
using int8_t   = signed char;
using uint8_t  = unsigned char;
using int16_t  = signed short;
using uint16_t = unsigned short;
using int32_t  = signed int;
using uint32_t = unsigned int;
using int64_t  = signed long long;
using uint64_t = unsigned long long;

#if _RHLIB_BITNESS == 64
using uintptr_t = uint64_t;
using intptr_t  = int64_t;
#else
using uintptr_t = uint32_t;
using intptr_t  = int32_t;
#endif

using size_t    = decltype(sizeof(int));
using ptrdiff_t = decltype(((int*)0) - ((int*)0));
using ssize_t   = ptrdiff_t;
using nullptr_t = decltype(nullptr);

// works just like a number, but doesn't collapse with "char"
struct byte {
  uint8_t value;

  constexpr byte() noexcept
    : value(0) {}

  template <typename T>
  constexpr byte(T&& arg) noexcept
    : value(static_cast<uint8_t>(arg)) {}

  constexpr operator int8_t  () const noexcept { return value; }
  constexpr operator uint8_t () const noexcept { return value; }
  constexpr operator int16_t () const noexcept { return value; }
  constexpr operator uint16_t() const noexcept { return value; }
  constexpr operator int32_t () const noexcept { return value; }
  constexpr operator uint32_t() const noexcept { return value; }
  constexpr operator int64_t () const noexcept { return value; }
  constexpr operator uint64_t() const noexcept { return value; }

  template <typename T>
  constexpr byte& operator+=(T&& right)  { value += right; return *this; }
  constexpr byte& operator+=(byte right) { value += right.value; return *this; }
  
  template <typename T>
  constexpr byte& operator-=(T&& right)  { value -= right; return *this; }
  constexpr byte& operator-=(byte right) { value -= right.value; return *this; }
  
  template <typename T>
  constexpr byte& operator*=(T&& right)  { value *= right; return *this; }
  constexpr byte& operator*=(byte right) { value *= right.value; return *this; }
  
  template <typename T>
  constexpr byte& operator/=(T&& right)  { value /= right; return *this; }
  constexpr byte& operator/=(byte right) { value /= right.value; return *this; }
  
  template <typename T>
  constexpr byte& operator%=(T&& right)  { value %= right; return *this; }
  constexpr byte& operator%=(byte right) { value %= right.value; return *this; }
};

using word  = uint16_t;
using dword = uint32_t;
using qword = uint64_t;

template <typename InIt, typename OutIt>
constexpr void copy(OutIt dest, InIt begin, InIt end) {
  for (auto it = begin; it != end; ++it, ++dest)
    *dest = *it;
}

template <typename InIt, typename OutIt>
constexpr void copy(OutIt dest, InIt data, size_t count) {
  for (size_t idx = 0; idx < count; ++idx)
    dest[idx] = data[idx];
}

template <typename T>
constexpr auto min(T const& left) {
  return left;
}

template <typename T>
constexpr auto min(T const& left, T const& others...) {
  T const& temp = min<T>(others);
  return left < temp ? left : temp;
}

template <typename T>
constexpr auto max(T const& left) {
  return left;
}

template <typename T>
constexpr auto max(T const& left, T const& others...) {
  T const& temp = max<T>(others);
  return left > temp ? left : temp;
}

template <typename T>
constexpr auto clamp(T const& value, T const& min_border, T const& max_border) {
  return min(max(value, min_border), max_border);
}

template<typename T>
T&& declval() noexcept {
  static_assert(false, "don't use declval in evaluated code");
}

_RHLIB_END

constexpr rh::byte operator+(rh::byte left, rh::byte right) noexcept {
  return rh::byte(left.value + right.value);
}
constexpr rh::byte operator-(rh::byte left, rh::byte right) noexcept {
  return rh::byte(left.value - right.value);
}
constexpr rh::byte operator*(rh::byte left, rh::byte right) noexcept {
  return rh::byte(left.value * right.value);
}
constexpr rh::byte operator/(rh::byte left, rh::byte right) noexcept {
  return rh::byte(left.value / right.value);
}
constexpr rh::byte operator%(rh::byte left, rh::byte right) noexcept {
  return rh::byte(left.value % right.value);
}
