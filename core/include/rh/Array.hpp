#pragma once
#define _RHLIB_INCLUDED_ARRAY

#include <rh.hpp>

#include <rh/InitList.hpp>

_RHLIB_BEGIN

template <typename T, size_t Count>
class Array {
private:
  using value_type = T;

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
  [[nodiscard]]
  constexpr T* data() noexcept {
    return m_values;
  }
  
  [[nodiscard]]
  constexpr T const* data() const noexcept {
    return m_values;
  }

  [[nodiscard]]
  constexpr T* begin() noexcept {
    return m_values;
  }

  [[nodiscard]]
  constexpr T const* begin() const noexcept {
    return m_values;
  }

  [[nodiscard]]
  constexpr T* end() noexcept {
    return m_values + count();
  }

  [[nodiscard]]
  constexpr T const* end() const noexcept {
    return m_values + count();
  }

  [[nodiscard]]
  constexpr bool isEmpty() const noexcept {
    return false;
  }

  [[nodiscard]]
  constexpr size_t length() const noexcept {
    return Count;
  }

  [[nodiscard]]
  constexpr T& operator[](size_t index) noexcept {
    return m_values[index];
  }

  [[nodiscard]]
  constexpr T const& operator[](size_t index) const noexcept {
    return m_values[index];
  }

private:
  constexpr void _copyOther(Array const& other) noexcept {
    for (size_t i = 0; i < Count; ++i)
      m_values[i] = other[i];
  }

  constexpr void _stealOther(Array& other) noexcept {
    for (size_t i = 0; i < Count; ++i)
      m_values[i] = move(other[i]);
  }

private:
  T m_values[Count];
};

_RHLIB_END

_RHLIB_GLOBAL_CLASS(Array);
