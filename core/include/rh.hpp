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

#define _RHLIB_BEGIN        namespace rh {
#define _RHLIB_END          }

#define _RHLIBH             ::rh::_Hidden::
#define _RHLIB_HIDDEN_BEGIN namespace _Hidden {
#define _RHLIB_HIDDEN_END   }

#ifndef _RHLIB_NO_GLOBALS
# define _RHLIB_GLOBAL_CLASS(clazz)  using rh::clazz;
# define _RHLIB_GLOBAL_NS(ns)        namespace ns = rh::ns;
#else
# define _RHLIB_GLOBAL_CLASS(...)
# define _RHLIB_GLOBAL_NS(...)
#endif

#define _RHLIB_NODISCARD [[nodiscard]]
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

_RHLIB_BEGIN

enum class OSType {};

static constexpr OSType OS_UNSUPPORTED = static_cast<OSType>(_RHLIB_OS_UNSUPPORTED);
static constexpr OSType OS_WINDOWS     = static_cast<OSType>(_RHLIB_OS_WINDOWS);
static constexpr OSType OS             = static_cast<OSType>(_RHLIB_OS);

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

using int8_t    = signed char;
using uint8_t   = unsigned char;
using int16_t   = signed short;
using uint16_t  = unsigned short;
using int32_t   = signed int;
using uint32_t  = unsigned int;
using int64_t   = signed long long;
using uint64_t  = unsigned long long;

using uintptr_t = conditional<sizeof(void*) == 8, uint64_t, uint32_t>;
using intptr_t  = conditional<sizeof(void*) == 8, int64_t, int32_t>;

using size_t    = decltype(sizeof(int));
using ssize_t   = conditional<sizeof(size_t) == 8, int64_t, int32_t>;
using ptrdiff_t = decltype(((int*)0) - ((int*)0));
using nullptr_t = decltype(nullptr);

using byte_t    = uint8_t;
using uchar_t   = char32_t;

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

// Just like a std::launder
template <typename T>
_RHLIB_NODISCARD
constexpr T* launder(T* pointer) noexcept {
  return __builtin_launder(pointer);
}

_RHLIB_HIDDEN_BEGIN

template <class Owner, size_t Size>
struct pimpl {
  using data_type = typename Owner::Data;

  static_assert(Size > 0, "Invalid PIMPL size");

  byte_t storage[Size];

  template <typename... ArgsT>
  inline pimpl(ArgsT&&... args) {
    static_assert(sizeof(data_type) == Size, "Invalid PIMPL size");
    construct_at<data_type>((data_type*)storage, forward<ArgsT>(args)...);
  }

  inline ~pimpl() {
    static_assert(sizeof(data_type) == Size, "Invalid PIMPL size");
    destruct_at<data_type>((data_type*)storage);
  }

  inline data_type* operator->() noexcept {
    return (data_type*)storage;
  }
  
  inline data_type const* operator->() const noexcept {
    return (data_type const*)storage;
  }
};

_RHLIB_HIDDEN_END

#define _RHLIB_DEFINE_PIMPL(size_expr)                      \
  static constexpr ::rh::size_t _PimplDataSize = size_expr; \
  struct Data;                                              \
  friend _RHLIBH pimpl<type, _PimplDataSize>;               \
  _RHLIBH pimpl<type, _PimplDataSize> m

_RHLIB_END

template <typename T, typename = rh::enable_if<rh::is_flag_type<T>>>
constexpr T operator|(T left, T right) noexcept {
  using underlying_type = rh::underlying_type<T>;
  return T(static_cast<underlying_type>(left) | static_cast<underlying_type>(right));
}

template <typename T, typename = rh::enable_if<rh::is_flag_type<T>>>
constexpr auto operator&(T left, T right) noexcept {
  using underlying_type = rh::underlying_type<T>;
  return _RHLIBH flag_enum_class_wrapper<T>(static_cast<underlying_type>(left) & static_cast<underlying_type>(right));
}

template <typename T, typename = rh::enable_if<rh::is_flag_type<T>>>
constexpr T operator~(T left) noexcept {
  using underlying_type = rh::underlying_type<T>;
  return T(~static_cast<underlying_type>(left));
}

#ifndef __NOTHROW_T_DEFINED
#define __NOTHROW_T_DEFINED
namespace std {

struct nothrow_t {
  explicit nothrow_t() = default;
};

extern nothrow_t const nothrow;

} // namespace std
#endif

_RHLIB_NODISCARD _RHLIB_ALLOCATOR
void* __cdecl operator new(size_t size);
_RHLIB_NODISCARD _RHLIB_ALLOCATOR
void* __cdecl operator new(size_t size, std::nothrow_t const&) noexcept;
_RHLIB_NODISCARD _RHLIB_ALLOCATOR
void* __cdecl operator new[](size_t size);
_RHLIB_NODISCARD _RHLIB_ALLOCATOR
void* __cdecl operator new[](size_t size, std::nothrow_t const&) noexcept;
void __cdecl operator delete(void* block) noexcept;
void __cdecl operator delete(void* block, std::nothrow_t const&) noexcept;
void __cdecl operator delete[](void* block) noexcept;
void __cdecl operator delete[](void* block, std::nothrow_t const&) noexcept;
void __cdecl operator delete(void* block, rh::size_t size) noexcept;
void __cdecl operator delete[](void* block, rh::size_t size) noexcept;

#ifndef __PLACEMENT_NEW_INLINE
#define __PLACEMENT_NEW_INLINE

inline void* __cdecl operator new(size_t size, void* where) noexcept {
  _RHLIB_UNUSED(size);
  return where;
}
inline void __cdecl operator delete(void*, void*) noexcept {}

#endif

#ifndef __PLACEMENT_VEC_NEW_INLINE
#define __PLACEMENT_VEC_NEW_INLINE

inline void* __cdecl operator new[](size_t size, void* where) noexcept {
  _RHLIB_UNUSED(size);
  return where;
}
inline void __cdecl operator delete[](void*, void*) noexcept {}

#endif

_RHLIB_BEGIN

template <typename T, typename... ArgsT>
constexpr void construct_at(T* object, ArgsT&&... args) {
  new (object) T (forward<ArgsT>(args)...);
}

template <typename T>
constexpr void destruct_at(T* object) {
  object->~T();
}

_RHLIB_END
