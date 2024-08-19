#pragma once
#define _RHLIB_INCLUDED_MUTEX

#include <rh.hpp>

_RHLIB_BEGIN

class Mutex {
public:
  using type = Mutex;

public:
  Mutex() noexcept;
  ~Mutex() noexcept;

public:
  // can throw windows exception if deadlock is possible
  void lock();
  void unlock() noexcept;

  // true = locked, false = not locked
  bool try_lock() noexcept;

private:
#pragma pack(push, 8)
  struct Pimpl_Windows {
    // struct _RTL_CRITICAL_SECTION
    void* _0;     // PRTL_CRITICAL_SECTION_DEBUG DebugInfo;
    long _1;      // LONG LockCount;
    long _2;      // LONG RecursionCount;
    uintptr_t _3; // HANDLE OwningThread;
    uintptr_t _4; // HANDLE LockSemaphore;
    uintptr_t _5; // ULONG_PTR SpinCount;
  };
#pragma pack(pop)

  _RHLIB_DEFINE_PIMPL(
    OS == OS_WINDOWS ? sizeof(Pimpl_Windows) :
    0
  );
};

_RHLIB_END
