#pragma once
#define _RHLIB_INCLUDED_ARRAY

#include <rh.hpp>

#include <rh/InitList.hpp>

_RHLIB_BEGIN

template <typename T, size_t Count>
class Array {
private:
  using type = Array<T, Count>;

  using value_type      = T;
  using pointer         = T*;
  using const_pointer   = T const*;
  using reference       = T&;
  using const_reference = T const&;
  using size_type       = size_t;

public:
  constexpr Array() noexcept = default;

  constexpr Array(InitList<value_type> init) noexcept {
    copy(m_values, init.begin(), init.end());
  }
  
  constexpr Array(Array const& other) noexcept {
    _copyOther(other); 
  }

  constexpr Array(Array&& other) noexcept {
    _stealOther(other);
  }

  constexpr ~Array() {}

  constexpr Array& operator=(Array const& other) noexcept {
    _copyOther(other); 
    return *this;
  }

  constexpr Array& operator=(Array&& other) noexcept {
    _stealOther(other);
    return *this;
  }

public:
  _RHLIB_NODISCARD
  constexpr pointer data() noexcept {
    return m_values;
  }
  
  _RHLIB_NODISCARD
  constexpr const_pointer data() const noexcept {
    return m_values;
  }

  _RHLIB_NODISCARD
  constexpr pointer begin() noexcept {
    return m_values;
  }

  _RHLIB_NODISCARD
  constexpr const_pointer begin() const noexcept {
    return m_values;
  }

  _RHLIB_NODISCARD
  constexpr pointer end() noexcept {
    return m_values + count();
  }

  _RHLIB_NODISCARD
  constexpr const_pointer end() const noexcept {
    return m_values + count();
  }

  _RHLIB_NODISCARD
  constexpr size_type count() const noexcept {
    return Count;
  }

  _RHLIB_NODISCARD
  constexpr reference operator[](size_type index) noexcept {
    return m_values[index];
  }

  _RHLIB_NODISCARD
  constexpr const_reference operator[](size_type index) const noexcept {
    return m_values[index];
  }

private:
  constexpr void _copyOther(Array const& other) noexcept {
    for (size_type i = 0; i < Count; ++i)
      m_values[i] = other[i];
  }

  constexpr void _stealOther(Array& other) noexcept {
    for (size_type i = 0; i < Count; ++i)
      m_values[i] = move(other[i]);
  }

private:
  value_type m_values[Count];
};

_RHLIB_END

_RHLIB_GLOBAL_CLASS(Array);
