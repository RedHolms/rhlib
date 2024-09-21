#pragma once
#define _RHLIB_INCLUDED_INITLIST

#include <rh.hpp>

_RHLIB_BEGIN

template <typename T>
class InitList {
public:
  using type = InitList<T>;

  using value_type    = T;
  using const_pointer = T const*;
  using size_type     = size_t;

public:
  InitList() = delete;

  constexpr InitList(const_pointer first, const_pointer last) noexcept
    : m_begin(first), m_end(last) {}

public:
  [[nodiscard]]
  constexpr const_pointer begin() const noexcept {
    return m_begin;
  }
  
  [[nodiscard]]
  constexpr const_pointer end() const noexcept {
    return m_end;
  }

  [[nodiscard]]
  constexpr size_type count() const noexcept {
    return static_cast<size_type>(m_end - m_begin);
  }

private:
  const_pointer const m_begin;
  const_pointer const m_end;
};

_RHLIB_END

_RHLIB_GLOBAL_CLASS(InitList);
