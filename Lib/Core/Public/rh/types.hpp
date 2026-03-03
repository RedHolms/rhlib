#pragma once

namespace rh {

template <typename, typename>
inline constexpr bool is_same_type = false;

template <typename T>
inline constexpr bool is_same_type<T, T> = true;

template <typename T, typename... Types>
inline constexpr bool is_any_of_types = (is_same_type<T, Types> || ...);

} // namespace rh
