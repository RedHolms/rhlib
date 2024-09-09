#include <rh/Mutex.hpp>

#include <Windows.h>

using rh::Mutex;

struct Mutex::Data {
  CRITICAL_SECTION critical_section;
};

Mutex::Mutex() noexcept {
  InitializeCriticalSection(&m->critical_section);
}

Mutex::~Mutex() noexcept {
  DeleteCriticalSection(&m->critical_section);
}

void Mutex::lock() {
  EnterCriticalSection(&m->critical_section);
}

void Mutex::unlock() noexcept {
  LeaveCriticalSection(&m->critical_section);
}

bool Mutex::try_lock() noexcept {
  return TryEnterCriticalSection(&m->critical_section) != FALSE;
}
