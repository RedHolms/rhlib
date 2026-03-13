#include <rh/log/thread.hpp>

#include <Windows.h>

rh::log::ThreadInfo rh::log::GetCurrentThreadInfo() {
  ThreadInfo result {};

  result.id = GetCurrentThreadId();
  result.handle = GetCurrentThread();

  wchar_t* threadName;
  HRESULT hresult = GetThreadDescription(result.handle, &threadName);
  if (SUCCEEDED(hresult)) {
    result.name = threadName;
    LocalFree(threadName);
  }

  return result;
}
