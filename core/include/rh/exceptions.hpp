#pragma once
#define _RHLIB_INCLUDED_EXCEPTIONS

#include <rh.hpp>

#include <rh/String.hpp>
#include <rh/TypeInfo.hpp>

_RHLIB_BEGIN

// Base class
class Exception {
protected:
  constexpr Exception(TypeInfo type_info) noexcept
    : m_type_info(type_info) {}

public:
  _RHLIB_NODISCARD
  constexpr TypeInfo type_info() const noexcept {
    return m_type_info;
  }

  _RHLIB_NODISCARD
  constexpr const char* type_name() const noexcept {
    return m_type_info.name();
  }

  _RHLIB_NODISCARD
  constexpr size_t type_hash() const noexcept {
    return m_type_info.hash_code();
  }

  _RHLIB_NODISCARD
  virtual StringView info() const noexcept = 0;

private:
  TypeInfo m_type_info;
};

// Generic runtime exception
class RuntimeError : public Exception {
public:
  constexpr RuntimeError(StringView info) noexcept;

public:
  _RHLIB_NODISCARD
  inline StringView info() const noexcept {
    return m_info;
  }

private:
  String m_info;
};

constexpr RuntimeError::RuntimeError(StringView info) noexcept
  : m_info(info), Exception(typeid(RuntimeError)) {}

_RHLIB_END
