#pragma once
#define _RHLIB_INCLUDED_PAIR

#include <rh.hpp>

_RHLIB_BEGIN

template <typename T, typename K>
struct Pair {
  T first;
  K second;

  constexpr Pair() = default;

  constexpr Pair(T const& first, K const& second)
    : first(first), second(second) {}

  constexpr Pair(T&& first, K&& second)
    : first(move(first)), second(move(second)) {}

  constexpr T& getFirst() noexcept { return first; }
  constexpr T const& getFirst() const noexcept { return first; }

  constexpr K& getSecond() noexcept { return second; }
  constexpr K const& getSecond() const noexcept { return second; }
};

// Pair, where first type (T) can be empty without unnecessary memory use
template <typename T, typename K>
struct CompressedPair : public T {
  K _second;

  constexpr CompressedPair() = default;

  constexpr CompressedPair(T const& first, K const& second)
    : T(first), _second(second) {}

  constexpr CompressedPair(T&& first, K&& second)
    : T(move(first)), _second(move(second)) {}

  constexpr T& getFirst() noexcept { return *this; }
  constexpr T const& getFirst() const noexcept { return *this; }

  constexpr K& getSecond() noexcept { return _second; }
  constexpr K const& getSecond() const noexcept { return _second; }
};

_RHLIB_END
