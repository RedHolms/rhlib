#pragma once
#define _RHLIB_INCLUDED_ARRAY

#include <rh.hpp>
#include <rh/InitList.hpp>

_RHLIB_BEGIN

template <typename T, size_t Count>
class Array {
private:
  using value_type      = T;
  using pointer         = T*;
  using const_pointer   = T const*;
  using reference       = T&;
  using const_reference = T const&;
  using size_type       = size_t;

public:
  constexpr Array() = default;

  constexpr Array(InitList<value_type> init) noexcept {
    copy(m_values, init.begin(), init.end());
  }
  
  constexpr Array(Array const& other) noexcept {
    _copy_other(other); 
  }

  constexpr Array(Array&& other) noexcept {
    _steal_other(other);
  }

  constexpr ~Array() {}

  constexpr Array& operator=(Array const& other) noexcept {
    _copy_other(other); 
    return *this;
  }

  constexpr Array& operator=(Array&& other) noexcept {
    _steal_other(other);
    return *this;
  }

public:
  constexpr pointer data() noexcept {
    return m_values;
  }
  
  constexpr const_pointer data() const noexcept {
    return m_values;
  }

  constexpr size_type count() const noexcept {
    return Count;
  }

  constexpr reference operator[](size_type index) noexcept {
    return m_values[index];
  }

  constexpr const_reference operator[](size_type index) const noexcept {
    return m_values[index];
  }

private:
  constexpr void _copy_other(Array const& other) noexcept {
    for (size_type i = 0; i < Count; ++i)
      m_values[i] = other[i];
  }

  constexpr void _steal_other(Array& other) noexcept {
    for (size_type i = 0; i < Count; ++i)
      m_values[i] = move(other[i]);
  }

private:
  T m_values[Count];
};

_RHLIB_END

_RHLIB_GLOBAL_CLASS(Array)
