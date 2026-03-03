#pragma once

#include <rh/String.hpp>

String Win32ErrorToString(DWORD errorCode);

forceinline String GetLastErrorString() {
  return Win32ErrorToString(GetLastError());
}
