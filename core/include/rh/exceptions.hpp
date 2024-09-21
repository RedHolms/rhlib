#pragma once
#define _RHLIB_INCLUDED_EXCEPTIONS

#include <rh.hpp>

#include <rh/String.hpp>
#include <rh/TypeInfo.hpp>

#include <exception>

_RHLIB_BEGIN

// Base class
class Exception : public std::exception {
public:
  Exception() = delete;

protected:
  inline Exception(TypeInfo typeInfo) noexcept
    : m_typeInfo(typeInfo) {}

public:
  [[nodiscard]]
  constexpr TypeInfo typeInfo() const noexcept {
    return m_typeInfo;
  }

  [[nodiscard]]
  constexpr const char* typeName() const noexcept {
    return m_typeInfo.name();
  }

  [[nodiscard]]
  constexpr size_t typeHash() const noexcept {
    return m_typeInfo.hash_code();
  }

  [[nodiscard]]
  virtual StringView info() const noexcept = 0;

  // compatibility with STL
  [[nodiscard]]
  constexpr const char* what() const noexcept { return "rhlib exception"; }

private:
  TypeInfo m_typeInfo;
};

// Generic runtime exception
class RuntimeError : public Exception {
public:
  inline RuntimeError(StringView info) noexcept;

protected:
  inline RuntimeError(StringView info, TypeInfo typeInfo) noexcept
    : m_info(info), Exception(typeInfo) {}

public:
  [[nodiscard]]
  inline StringView info() const noexcept {
    return m_info;
  }

private:
  String m_info;
};

inline RuntimeError::RuntimeError(StringView info) noexcept
  : RuntimeError(info, typeid(RuntimeError)) {}

// Invalid index
class IndexError final : public RuntimeError {
public:
  inline IndexError(StringView info = U"invalid index") noexcept;
};

inline IndexError::IndexError(StringView info) noexcept
  : RuntimeError(info, typeid(IndexError)) {}

_RHLIB_END
