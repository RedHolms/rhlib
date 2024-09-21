#pragma once
#define _RHLIB_INCLUDED_ANYPTR

#include <rh.hpp>

_RHLIB_BEGIN

struct ConstAnyPtr {
  uintptr_t value = 0;

  constexpr ConstAnyPtr() noexcept = default;

  template <typename T>
  constexpr ConstAnyPtr(T value) noexcept
    : value(static_cast<uintptr_t>(value)) {}

  inline ConstAnyPtr(void const* ptr) noexcept
    : value(reinterpret_cast<uintptr_t>(ptr)) {}

  template <typename T>
  [[nodiscard]]
  inline T const* get() const noexcept {
    return reinterpret_cast<T*>(value);
  }

  constexpr operator uintptr_t() const noexcept {
    return value;
  }

  constexpr operator ptrdiff_t() const noexcept {
    return static_cast<ptrdiff_t>(value);
  }

  inline operator void const*() const noexcept {
    return reinterpret_cast<void const*>(value);
  }

  constexpr ConstAnyPtr& operator++() noexcept {
    ++value;
    return *this;
  }

  constexpr ConstAnyPtr& operator--() noexcept {
    --value;
    return *this;
  }

  [[nodiscard]]
  constexpr ptrdiff_t operator-(ptrdiff_t other) const noexcept {
    return static_cast<ptrdiff_t>(value) - other;
  }

  [[nodiscard]]
  constexpr uintptr_t operator+(ptrdiff_t other) const noexcept {
    return value + other;
  }
};

struct AnyPtr : public ConstAnyPtr {
  using ConstAnyPtr::ConstAnyPtr;

  inline AnyPtr(void* ptr) noexcept
    : ConstAnyPtr(reinterpret_cast<uintptr_t>(ptr)) {}

  template <typename T>
  [[nodiscard]]
  inline T* get() const noexcept {
    return reinterpret_cast<T*>(value);
  }

  inline operator void*() const noexcept {
    return reinterpret_cast<void*>(value);
  }
};

_RHLIB_END

_RHLIB_GLOBAL_CLASS(ConstAnyPtr);
_RHLIB_GLOBAL_CLASS(AnyPtr);
