#pragma once
#define _RHLIB_INCLUDED

#ifdef _WINDOWS_
#warning Consider including <Windows.h> after rhlib headers
#undef min
#undef max
#endif

#if __cplusplus < 202302L
# error rhstd requires at least C++23
#endif

#if !defined(__has_extension) || !defined(__has_feature)
# error Invalid compiler
#endif

#define _RHLIB_             rh
#define _RHLIB              ::_RHLIB_::
#define _RHLIB_BEGIN        namespace _RHLIB_ {
#define _RHLIB_END          }

#define _RHLIBH_            _Hidden
#define _RHLIBH             ::_RHLIB_::_RHLIBH_::
#define _RHLIB_HIDDEN_BEGIN namespace _RHLIBH_ {
#define _RHLIB_HIDDEN_END   }

#ifndef _RHLIB_NO_GLOBALS
# define _RHLIB_GLOBAL_CLASS(clazz)  using _RHLIB clazz;
# define _RHLIB_GLOBAL_NS(ns)        namespace ns = _RHLIB ns;
#else
# define _RHLIB_GLOBAL_CLASS(...)
# define _RHLIB_GLOBAL_NS(...)
#endif

#define _RHLIB_NODISCARD [[nodiscard]]

#define _RHLIB_API extern

#define _RHLIB_OS_WINDOWS     0
#define _RHLIB_OS_GNU_LINUX   1
#define _RHLIB_OS_UNSUPPORTED -1

#if defined(_WIN32)
# define _RHLIB_OS _RHLIB_OS_WINDOWS
#elif defined(__gnu_linux__)
# define _RHLIB_OS _RHLIB_OS_GNU_LINUX
#else
# define _RHLIB_OS _RHLIB_OS_UNSUPPORTED
#endif

_RHLIB_BEGIN

enum OSType {
  OS_WINDOWS     = _RHLIB_OS_WINDOWS,
  OS_GNU_LINUX   = _RHLIB_OS_GNU_LINUX,
  OS_UNSUPPORTED = _RHLIB_OS_UNSUPPORTED
};

static constexpr OSType OS = static_cast<OSType>(_RHLIB_OS);

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

template <typename, typename>
static constexpr bool is_same_type = false;

template <typename T>
static constexpr bool is_same_type<T, T> = false;

template <typename>
static constexpr bool is_lvalue_reference = false;

template <typename T>
static constexpr bool is_lvalue_reference<T&&> = true;

template <typename T>
static constexpr auto ssizeof = sizeof(T);

template <>
static constexpr decltype(sizeof(int)) ssizeof<void> = 0;

_RHLIB_HIDDEN_BEGIN

template <typename T>
struct remove_reference_wrapper
  : type_wrapper<T> {};
  
template <typename T>
struct remove_reference_wrapper<T&>
  : type_wrapper<T> {};
  
template <typename T>
struct remove_reference_wrapper<T&&>
  : type_wrapper<T> {};

_RHLIB_HIDDEN_END

template <typename T>
using remove_reference = unwrap_type<_RHLIBH remove_reference_wrapper<T>>;

_RHLIB_HIDDEN_BEGIN

template <bool Condition, typename TrueT, typename FalseT>
struct conditional_wrapper;

template <typename TrueT, typename FalseT>
struct conditional_wrapper<true, TrueT, FalseT>
  : type_wrapper<TrueT> {};

template <typename TrueT, typename FalseT>
struct conditional_wrapper<false, TrueT, FalseT>
  : type_wrapper<FalseT> {};

_RHLIB_HIDDEN_END

template <bool Condition, typename TrueT, typename FalseT>
using conditional = unwrap_type<_RHLIBH conditional_wrapper<Condition, TrueT, FalseT>>;

_RHLIB_HIDDEN_BEGIN

template <bool Condition>
struct enable_if_wrapper;

template <>
struct enable_if_wrapper<true>
  : type_wrapper<void> {};

_RHLIB_HIDDEN_END

template <bool Condition>
using enable_if = unwrap_type<_RHLIBH enable_if_wrapper<Condition>>;

_RHLIB_HIDDEN_BEGIN

template <int Size, typename... Candidates>
struct find_type_with_size_wrapper;

template <int Size, typename T, typename... Other>
struct find_type_with_size_wrapper<Size, T, Other...> {
  using next = find_type_with_size_wrapper<Size, Other...>;
  using type = typename conditional_wrapper<sizeof(T) == Size, T, typename next::type>::type;
};

template <int Size>
struct find_type_with_size_wrapper<Size> {
  using type = void;
};

_RHLIB_HIDDEN_END

template <int Size, typename... Candidates>
using find_type_with_size = typename _RHLIBH find_type_with_size_wrapper<Size, Candidates...>::type;

template <int Size>
using find_signed_integral_type_with_size = find_type_with_size<
  Size,
  signed char,
  signed short,
  signed int,
  signed long,
  signed long long
>;

template <int Size>
using find_unsigned_integral_type_with_size = find_type_with_size<
  Size,
  unsigned char,
  unsigned short,
  unsigned int,
  unsigned long,
  unsigned long long
>;

using int8_t    = find_signed_integral_type_with_size<1>;
using uint8_t   = find_unsigned_integral_type_with_size<1>;
using int16_t   = find_signed_integral_type_with_size<2>;
using uint16_t  = find_unsigned_integral_type_with_size<2>;
using int32_t   = find_signed_integral_type_with_size<4>;
using uint32_t  = find_unsigned_integral_type_with_size<4>;
using int64_t   = find_signed_integral_type_with_size<8>;
using uint64_t  = find_unsigned_integral_type_with_size<8>;
using uintptr_t = find_unsigned_integral_type_with_size<sizeof(void*)>;
using intptr_t  = find_signed_integral_type_with_size<sizeof(void*)>;

static_assert(!is_same_type<int8_t, void>, "Can't find valid type to use as int8_t");
static_assert(!is_same_type<uint8_t, void>, "Can't find valid type to use as uint8_t");
static_assert(!is_same_type<int16_t, void>, "Can't find valid type to use as int16_t");
static_assert(!is_same_type<uint16_t, void>, "Can't find valid type to use as uint16_t");
static_assert(!is_same_type<int32_t, void>, "Can't find valid type to use as int32_t");
static_assert(!is_same_type<uint32_t, void>, "Can't find valid type to use as uint32_t");
static_assert(!is_same_type<int64_t, void>, "Can't find valid type to use as int64_t");
static_assert(!is_same_type<uint64_t, void>, "Can't find valid type to use as uint64_t");
static_assert(!is_same_type<uintptr_t, void>, "Can't find valid type to use as uintptr_t");
static_assert(!is_same_type<intptr_t, void>, "Can't find valid type to use as intptr_t");

using size_t    = decltype(sizeof(int));
using ptrdiff_t = intptr_t;
using byte_t    = uint8_t;
using uchar_t   = char32_t;
using nullptr_t = decltype(nullptr);

template <typename T>
using underlying_type = __underlying_type(T);

_RHLIB_HIDDEN_BEGIN

template <typename T, typename... Types>
struct is_any_type_of_wrapper;

template <typename T, typename K, typename... Types>
struct is_any_type_of_wrapper<T, K, Types...> {
  static constexpr bool value = is_any_type_of_wrapper<T, Types...>::value;
};

template <typename T, typename... Types>
struct is_any_type_of_wrapper<T, T, Types...> {
  static constexpr bool value = true;
};

template <typename T>
struct is_any_type_of_wrapper<T> {
  static constexpr bool value = false;
};

_RHLIB_HIDDEN_END

template <typename T, typename... Types>
static constexpr bool is_any_type_of = _RHLIBH is_any_type_of_wrapper<T, Types...>::value;

template <typename T>
static constexpr bool is_integral_type = is_any_type_of<T,
  int8_t,  uint8_t,
  int16_t, uint16_t,
  int32_t, uint32_t,
  int64_t, uint64_t
>;

template <typename T>
static constexpr bool is_char_type = is_any_type_of<T,
  char, wchar_t,
  char8_t, char16_t, char32_t,
  uchar_t
>;

template <typename T>
_RHLIB_NODISCARD
constexpr T&& forward(remove_reference<T>& arg) noexcept {
  return static_cast<T&&>(arg);
}

template <typename T>
_RHLIB_NODISCARD
constexpr T&& forward(remove_reference<T>&& arg) noexcept {
  static_assert(!is_lvalue_reference<T>, "bad forward call");
  return static_cast<T&&>(arg);
}

template <typename T>
_RHLIB_NODISCARD
constexpr auto move(T&& arg) noexcept {
  return static_cast<remove_reference<T>&&>(arg);
}

template <typename T, typename... ArgsT>
constexpr void construct_at(T* object, ArgsT&&... args) {
  new (object) T (forward<ArgsT>(args)...);
}

template <typename T>
constexpr void destruct_at(T* object) {
  object->~T();
}

template <typename InIt1, typename InIt2, typename OutIt>
constexpr void copy(OutIt dest, InIt1 begin, InIt2 end) {
  for (auto it = begin; it != end; ++it, ++dest)
    *dest = *it;
}

template <typename InIt, typename OutIt>
constexpr void copy(OutIt dest, InIt data, size_t count) {
  for (size_t idx = 0; idx < count; ++idx)
    dest[idx] = data[idx];
}

template <typename T>
constexpr auto min(T const& left, T const& right) {
  return left < right ? left : right;
}

template <typename T>
constexpr auto max(T const& left, T const& right) {
  return left > right ? left : right;
}

template <typename T>
constexpr auto clamp(T const& value, T const& min_border, T const& max_border) {
  return min(max(value, min_border), max_border);
}

template <typename T, typename K>
struct pair {
  T first;
  K second;

  constexpr pair() = default;

  constexpr pair(T const& first, K const& second)
    : first(first), second(second) {}

  constexpr pair(T&& first, K&& second)
    : first(move(first)), second(move(second)) {}
};

template <typename>
static constexpr bool is_flag_type = false;

_RHLIB_HIDDEN_BEGIN

template <typename T>
struct flag_enum_class_wrapper {
  using type = T;
  using underlying_type = _RHLIB underlying_type<T>;

  underlying_type value;

  constexpr flag_enum_class_wrapper(underlying_type value) noexcept
    : value(value) {}

  constexpr flag_enum_class_wrapper(type value) noexcept
    : value(static_cast<underlying_type>(value)) {}
    
  constexpr operator underlying_type() const noexcept {
    return value;
  }
    
  constexpr operator type() const noexcept {
    return static_cast<type>(value);
  }

  constexpr operator bool() const noexcept {
    return value != 0;
  }
};

_RHLIB_HIDDEN_END

_RHLIB_END

template <typename T, typename = _RHLIB enable_if<_RHLIB is_flag_type<T>>>
constexpr T operator|(T left, T right) noexcept {
  using underlying_type = _RHLIB underlying_type<T>;
  return T(static_cast<underlying_type>(left) | static_cast<underlying_type>(right));
}

template <typename T, typename = _RHLIB enable_if<_RHLIB is_flag_type<T>>>
constexpr auto operator&(T left, T right) noexcept {
  using underlying_type = _RHLIB underlying_type<T>;
  return _RHLIBH flag_enum_class_wrapper<T>(static_cast<underlying_type>(left) & static_cast<underlying_type>(right));
}

template <typename T, typename = _RHLIB enable_if<_RHLIB is_flag_type<T>>>
constexpr T operator~(T left) noexcept {
  using underlying_type = _RHLIB underlying_type<T>;
  return T(~static_cast<underlying_type>(left));
}
