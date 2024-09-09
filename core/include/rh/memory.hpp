#pragma once
#define _RHLIB_INCLUDED_MEMORY

#include <rh.hpp>

#include <rh/AnyPtr.hpp>
#include <rh/TypeTraits.hpp>

_RHLIB_BEGIN

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
access getAccess(AnyPtr address, size_t bytes_count);

_RHLIB_API
void setAccess(access value, AnyPtr address, size_t bytes_count);

} // namespace memory

_RHLIB_MAKE_ENUM_FLAGS(memory::access)

namespace memory {

struct AccessScope {
  AnyPtr address;
  size_t bytesCount;
  access prevAccess;

  inline AccessScope(access value, AnyPtr address, size_t bytes_count)
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
  inline AddAccessScope(access value, AnyPtr address, size_t bytes_count)
    : AccessScope(
      getAccess(address, bytes_count) | value,
      address, bytes_count
    )
  {}
};

struct WriteAccessScope : AddAccessScope {
  inline WriteAccessScope(AnyPtr address, size_t bytes_count)
    : AddAccessScope(access::write, address, bytes_count)
  {}
};

struct FullAccessScope : AccessScope {
  inline FullAccessScope(AnyPtr address, size_t bytes_count)
    : AccessScope(access::full, address, bytes_count)
  {}
};

template <typename T = byte_t>
inline void copy(AnyPtr destination, ConstAnyPtr source, size_t elements_count) {
  auto dst = destination.get<T>();
  auto src = source.get<T>();

  if constexpr (destination - source >= (elements_count * sizeof(T))) {
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
inline void fill(AnyPtr destination, T value, size_t elements_count) {
  auto dst = destination.get<T>();

  for (size_t i = 0; i < elements_count; ++i)
    dst[i] = value;
}

} // namespace memory

_RHLIB_END
