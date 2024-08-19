#pragma once
#define _RHLIB_INCLUDED_MEMORY

#include <rh.hpp>

_RHLIB_BEGIN

struct GenericConstPointer {
  uintptr_t value = 0;

  constexpr GenericConstPointer() noexcept = default;

  template <typename T, typename = enable_if<is_integral_type<T>>>
  constexpr GenericConstPointer(T value) noexcept
    : value(static_cast<uintptr_t>(value)) {}

  inline GenericConstPointer(void const* ptr) noexcept
    : value(reinterpret_cast<uintptr_t>(ptr)) {}

  template <typename T>
  _RHLIB_NODISCARD
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

  constexpr GenericConstPointer& operator++() noexcept {
    ++value;
    return *this;
  }

  constexpr GenericConstPointer& operator--() noexcept {
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

struct GenericPointer : public GenericConstPointer {
  using GenericConstPointer::GenericConstPointer;

  inline GenericPointer(void* ptr) noexcept
    : GenericConstPointer(reinterpret_cast<uintptr_t>(ptr)) {}

  template <typename T>
  _RHLIB_NODISCARD
  inline T* get() const noexcept {
    return reinterpret_cast<T*>(value);
  }
};

namespace memory {

enum class Access : uint8_t {
  none    = 0,
  read    = 1 << 0,
  write   = 1 << 1,
  execute = 1 << 2,

  full = read | write | execute
};

_RHLIB_NODISCARD
_RHLIB_API
Access getAccess(GenericConstPointer address, size_t bytes_count);

_RHLIB_API
void setAccess(Access value, GenericConstPointer address, size_t bytes_count);

} // namespace memory

template <>
static constexpr bool is_flag_type<memory::Access> = true;

namespace memory {

struct AccessScope {
  GenericConstPointer address;
  size_t bytesCount;
  Access prevAccess;

  inline AccessScope(Access value, GenericConstPointer address, size_t bytes_count)
    : address(address),
      bytesCount(bytes_count),
      prevAccess(getAccess(address, bytes_count))
  {
    setAccess(value, address, bytes_count);
  }

  inline ~AccessScope() {
    setAccess(prevAccess, address, bytesCount);
  }
};

struct AddAccessScope : AccessScope {
  inline AddAccessScope(Access value, GenericConstPointer address, size_t bytes_count)
    : AccessScope(
      getAccess(address, bytes_count) | value,
      address, bytes_count
    )
  {}
};

struct WriteAccessScope : AddAccessScope {
  inline WriteAccessScope(GenericConstPointer address, size_t bytes_count)
    : AddAccessScope(Access::write, address, bytes_count)
  {}
};

struct FullAccessScope : AccessScope {
  inline FullAccessScope(GenericConstPointer address, size_t bytes_count)
    : AccessScope(Access::full, address, bytes_count)
  {}
};

template <typename T = byte_t>
inline void copy(GenericPointer destin, GenericConstPointer source, size_t elements_count) {
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
inline void fill(GenericPointer destin, T value, size_t elements_count) {
  auto dst = destin.get<T>();

  for (size_t i = 0; i < elements_count; ++i)
    dst[i] = value;
}

} // namespace memory

_RHLIB_END
