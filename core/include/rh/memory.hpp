#pragma once
#define _RHLIB_INCLUDED_MEMORY

#include <rh.hpp>

_RHLIB_BEGIN

struct generic_const_pointer {
  uintptr_t value = 0;

  constexpr generic_const_pointer() noexcept = default;

  template <typename T, typename = enable_if<is_integral_type<T>>>
  constexpr generic_const_pointer(T value) noexcept
    : value(static_cast<uintptr_t>(value)) {}

  inline generic_const_pointer(void const* ptr) noexcept
    : value(reinterpret_cast<uintptr_t>(ptr)) {}

  template <typename T>
  inline T const* get() const noexcept {
    return reinterpret_cast<T*>(value);
  }

  constexpr operator uintptr_t() const noexcept {
    return value;
  }

  constexpr operator ptrdiff_t() const noexcept {
    return static_cast<ptrdiff_t>(value);
  }

  inline operator void*() const noexcept {
    return reinterpret_cast<void*>(value);
  }

  constexpr generic_const_pointer& operator++() noexcept {
    ++value;
    return *this;
  }

  constexpr generic_const_pointer& operator--() noexcept {
    --value;
    return *this;
  }

  constexpr ptrdiff_t operator-(ptrdiff_t other) const noexcept {
    return static_cast<ptrdiff_t>(value) - other;
  }
  
  constexpr uintptr_t operator+(ptrdiff_t other) const noexcept {
    return value + other;
  }
};

struct generic_pointer : public generic_const_pointer {
  using generic_const_pointer::generic_const_pointer;

  inline generic_pointer(void* ptr) noexcept
    : generic_const_pointer(reinterpret_cast<uintptr_t>(ptr)) {}

  template <typename T>
  inline T* get() const noexcept {
    return reinterpret_cast<T*>(value);
  }
};

namespace memory {

enum class access : uint8_t {
  none    = 0,
  read    = 1 << 0,
  write   = 1 << 1,
  execute = 1 << 2,

  full = read | write | execute
};

_RHLIB_NODISCARD
_RHLIB_API
access get_access(generic_const_pointer address, size_t bytes_count);

_RHLIB_API
void set_access(access value, generic_const_pointer address, size_t bytes_count);

} // namespace memory

template <>
static constexpr bool is_flag_type<memory::access> = true;

namespace memory {

struct access_scope {
  generic_const_pointer address;
  size_t bytes_count;
  access prev_access;

  inline access_scope(access value, generic_const_pointer address, size_t bytes_count)
    : address(address),
      bytes_count(bytes_count),
      prev_access(get_access(address, bytes_count))
  {
    set_access(value, address, bytes_count);
  }

  inline ~access_scope() {
    set_access(prev_access, address, bytes_count);
  }
};

struct add_access_scope : access_scope {
  inline add_access_scope(access value, generic_const_pointer address, size_t bytes_count)
    : access_scope(
      get_access(address, bytes_count) | value,
      address, bytes_count
    )
  {}
};

struct write_access_scope : add_access_scope {
  inline write_access_scope(generic_const_pointer address, size_t bytes_count)
    : add_access_scope(access::write, address, bytes_count)
  {}
};

struct full_access_scope : access_scope {
  inline full_access_scope(generic_const_pointer address, size_t bytes_count)
    : access_scope(access::full, address, bytes_count)
  {}
};

template <typename T = byte_t>
inline void copy(generic_pointer destin, generic_const_pointer source, size_t elements_count) {
  auto dst = destin.get<T>();
  auto src = source.get<T>();

  if constexpr (destin - source >= (elements_count * sizeof(T))) {
    // not overlapping
    for (size_t i = 0; i < elements_count; ++i)
      dst[i] = src[i];
  }
  else {
    // overlapping. need temporary buffer
    T* buffer = new T[elements_count];

    for (size_t i = 0; i < elements_count; ++i)
      buffer[i] = src[i];
    for (size_t i = 0; i < elements_count; ++i)
      dst[i] = buffer[i];

    delete[] buffer;
  }
}

template <typename T = byte_t>
inline void fill(generic_pointer destin, T value, size_t elements_count) {
  auto dst = destin.get<T>();

  for (size_t i = 0; i < elements_count; ++i)
    dst[i] = value;
}

} // namespace memory

_RHLIB_END

_RHLIB_GLOBAL_NS(memory)
