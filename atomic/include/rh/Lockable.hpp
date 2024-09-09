#pragma once
#define _RHLIB_INCLUDED_LOCKABLE

#include <rh.hpp>

_RHLIB_BEGIN

template <typename T>
concept Lockable = requires(T object) {
  object.lock();
  object.unlock();
  object.try_lock();
};

_RHLIB_END
