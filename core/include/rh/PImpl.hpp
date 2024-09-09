#pragma once
#define _RHLIB_INCLUDED_PIMPL

#include <rh.hpp>

#include <rh/TypeTraits.hpp> // for constuct_at and destruct_at, even if they're used only in source file

_RHLIB_BEGIN
_RHLIB_HIDDEN_BEGIN

template <typename RealT, typename StorageT>
struct PImpl {
  StorageT storage;

  template <typename... ArgsT>
  inline PImpl(ArgsT&&... args) {
    static_assert(sizeof(RealT) <= sizeof(StorageT), "Invalid PIMPL size");
    construct_at<RealT>((RealT*)&storage, forward<ArgsT>(args)...);
  }

  inline ~PImpl() {
    destruct_at<RealT>((RealT*)&storage);
  }

  inline RealT* operator->() noexcept {
    return (RealT*)&storage;
  }
  
  inline RealT const* operator->() const noexcept {
    return (RealT const*)storage;
  }
};

_RHLIB_HIDDEN_END
_RHLIB_END

#define _RHLIB_DEFINE_PIMPL(StorageT) \
  struct Data;                        \
  _RHLIBH PImpl<Data, StorageT> m

#if _RHLIB_OS == _RHLIB_OS_WINDOWS
#define _RHLIB_DEFINE_OS_PIMPL _RHLIB_DEFINE_PIMPL(WindowsData)
#endif
