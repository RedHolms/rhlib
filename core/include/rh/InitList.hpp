#pragma once
#define _RHLIB_INCLUDED_INITLIST

#include <rh.hpp>

_RHLIB_BEGIN

template <typename T>
class InitList {
public:
  using value_type = T;
  using pointer    = T const*;

  using size_type  = size_t;

public:
  InitList() = delete;

  constexpr InitList(pointer first, pointer last) noexcept
    : m_begin(first), m_end(last) {}

public:
  _RHLIB_NODISCARD
  pointer begin() const noexcept {
    return m_begin;
  }
  
  _RHLIB_NODISCARD
  pointer end() const noexcept {
    return m_end;
  }

  _RHLIB_NODISCARD
  size_type count() const noexcept {
    return static_cast<size_type>(m_end - m_begin);
  }

private:
  pointer const m_begin;
  pointer const m_end;
};

_RHLIB_END

_RHLIB_GLOBAL_CLASS(InitList)
