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

// Pair, where types can be empty without unnecessary memory usage:
//  if type is empty, and we're creating a variable of that type,
//  this variable always will take more than 0 bytes, i.e.:
//    struct A {};
//    A v;
//    static_assert(sizeof(v) > 0);
// But if we're derive from empty type, this wont take extra space
template <typename T, typename K>
struct CompressedPair : public T, K {
  constexpr CompressedPair() = default;

  constexpr CompressedPair(T const& first, K const& second)
    : T(first), K(second) {}

  constexpr CompressedPair(T&& first, K&& second)
    : T(move(first)), K(move(second)) {}

  constexpr T& getFirst() noexcept { return *this; }
  constexpr T const& getFirst() const noexcept { return *this; }

  constexpr K& getSecond() noexcept { return *this; }
  constexpr K const& getSecond() const noexcept { return *this; }
};

_RHLIB_END
