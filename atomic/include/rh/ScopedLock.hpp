#pragma once
#define _RHLIB_INCLUDED_SCOPEDLOCK

#include <rh.hpp>
#include <rh/Mutex.hpp>

_RHLIB_BEGIN

class ScopedLock {
public:
  using type = ScopedLock;

public:
  constexpr ScopedLock(Mutex& mtx)
    : m_mutex(mtx)
  {
    m_mutex.lock();
  }

  constexpr ~ScopedLock() {
    m_mutex.unlock();
  }

private:
  Mutex& m_mutex;
};

_RHLIB_END
