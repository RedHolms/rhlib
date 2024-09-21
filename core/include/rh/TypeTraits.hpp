#pragma once
#define _RHLIB_INCLUDED_TYPETRAITS

#include <rh.hpp>

#include <new> // for constructAt

_RHLIB_BEGIN

template <typename, typename>
static constexpr bool is_same_type = false;

template <typename T>
static constexpr bool is_same_type<T, T> = false;

template <typename T, typename Expected>
concept Exactly = is_same_type<T, Expected>;

template <typename T>
static constexpr bool is_void = is_same_type<T, void>;


template <typename From, typename To>
static constexpr bool is_convertible_to = 
  requires(From value) { declval<void(*)(To)>()(value); };

template <typename From, typename To>
concept ConvertibleTo = is_convertible_to<From, To>;

template <typename From, typename To>
static constexpr bool is_nothrow_convertible_to = 
  requires(From value) { { declval<void(*)(To)>()(value) } noexcept; };
  
template <typename From, typename To>
concept NothrowConvertibleTo = is_nothrow_convertible_to<From, To>;


template <typename>
static constexpr bool is_lvalue_reference = false;

template <typename T>
static constexpr bool is_lvalue_reference<T&> = true;

template <typename>
static constexpr bool is_rvalue_reference = false;

template <typename T>
static constexpr bool is_rvalue_reference<T&&> = true;

template <typename T>
static constexpr bool is_reference = is_lvalue_reference<T> || is_rvalue_reference<T>;


template <typename T>
static constexpr size_t ssizeof = sizeof(T);

template <>
static constexpr size_t ssizeof<void> = 0;


template <typename T>
using underlying_type = __underlying_type(T);


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

template <typename T, typename... Types>
struct is_any_type_of_wrapper;

template <typename T, typename K, typename... OtherTypes>
struct is_any_type_of_wrapper<T, K, OtherTypes...> {
  static constexpr bool value = is_any_type_of_wrapper<T, OtherTypes...>::value;
};

template <typename T, typename... OtherTypes>
struct is_any_type_of_wrapper<T, T, OtherTypes...> {
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
  signed char, unsigned char,
  signed short, unsigned short,
  signed int, unsigned int,
  signed long, unsigned long,
  signed long long, unsigned long long
>;

template <typename T>
static constexpr bool is_char_type = is_any_type_of<T,
  char, wchar_t, char8_t, char16_t, char32_t
>;


template <typename T>
[[nodiscard]]
constexpr auto move(T&& arg) noexcept {
  return static_cast<remove_reference<T>&&>(arg);
}

template <typename T>
[[nodiscard]]
constexpr T&& forward(remove_reference<T>& arg) noexcept {
  return static_cast<T&&>(arg);
}

template <typename T>
[[nodiscard]]
constexpr T&& forward(remove_reference<T>&& arg) noexcept {
  static_assert(!is_lvalue_reference<T>, "bad forward call");
  return static_cast<T&&>(arg);
}

template <typename T, typename... ArgsT>
constexpr void constructAt(T* object, ArgsT&&... args) {
  new (object) T (forward<ArgsT>(args)...);
}

template <typename T>
constexpr void destructAt(T* object) {
  object->~T();
}

_RHLIB_HIDDEN_BEGIN

template <typename>
static constexpr bool is_flags_enum_type = false;

template <typename T>
struct flag_enum_class_wrapper {
  using type = T;
  using underlying_type = underlying_type<T>;

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

#define _RHLIB_MAKE_ENUM_FLAGS(T)                     \
  _RHLIB_HIDDEN_BEGIN                                 \
  template <>                                         \
  static constexpr bool is_flags_enum_type<T> = true; \
  _RHLIB_HIDDEN_END

_RHLIB_END

template <typename T, typename = rh::enable_if<_RHLIBH is_flags_enum_type<T>>>
constexpr T operator|(T left, T right) noexcept {
  using underlying_type = rh::underlying_type<T>;
  return T(static_cast<underlying_type>(left) | static_cast<underlying_type>(right));
}

template <typename T, typename = rh::enable_if<_RHLIBH is_flags_enum_type<T>>>
constexpr auto operator&(T left, T right) noexcept {
  using underlying_type = rh::underlying_type<T>;
  return _RHLIBH flag_enum_class_wrapper<T>(static_cast<underlying_type>(left) & static_cast<underlying_type>(right));
}

template <typename T, typename = rh::enable_if<_RHLIBH is_flags_enum_type<T>>>
constexpr T operator~(T left) noexcept {
  using underlying_type = rh::underlying_type<T>;
  return T(~static_cast<underlying_type>(left));
}
