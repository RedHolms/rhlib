#pragma once

#include <bit>
#include <type_traits>

using std::bit_cast;

template <typename T>
constexpr T implicit_cast(std::type_identity_t<T> value) noexcept {
  return value;
}

template <typename T>
concept PointerLike =
  std::is_pointer_v<T> || std::is_same_v<T, uintptr_t> || std::is_same_v<T, ptrdiff_t>;

template <PointerLike T, PointerLike Y>
constexpr T ptr_cast(Y ptr) noexcept {
  return reinterpret_cast<T>(ptr);
}
